/*
 * A108145  Smallest prime consisting of n successive positive integers in
 *          descending order followed by a 9. a(3*k) = 0 as no such prime
 *          exists.
 * https://oeis.org/A108145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 3, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A108145: Smallest prime consisting of n successive positive
   integers in descending order followed by a 9.  a(3*k) = 0.
   Implementation uses a search over increasing starting integers
   and deterministic Miller–Rabin (deterministic for 64‑bit
   numbers, probabilistic for larger values). */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the number of decimal digits in a positive 64‑bit integer. */
static int num_digits_uint64(uint64_t x)
{
    int d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

/* Power of 10 as seq_t.  The table is constructed on first use. */
static seq_t pow10_seq(int d)
{
    static seq_t table[22];
    static bool ready = false;
    if (!ready) {
        table[0] = 1;
        for (int i = 1; i < 22; ++i) table[i] = table[i - 1] * 10;
        ready = true;
    }
    return table[d];
}

/* Modular exponentiation for arbitrary seq_t. */
static seq_t modpow_seq(seq_t a, seq_t e, seq_t mod)
{
    seq_t result = 1;
    seq_t base   = a % mod;
    seq_t exp    = e;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin for 64‑bit numbers, probabilistic for larger. */
static bool seq_is_probable_prime(seq_t n)
{
    if (n < 2) return false;
    if ((n & 1) == 0) return n == 2;
    static const uint64_t bases[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    seq_t d = n - 1;
    idx_t s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        if ((seq_t)bases[i] >= n - 2) continue;
        seq_t a = (seq_t)bases[i];
        seq_t x = modpow_seq(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (idx_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Main function computing A108145 */
seq_t a(idx_t n)
{
    /* n multiples of 3 have no such prime */
    if (n % 3 == 0) return 0;

    /* Search over starting integers m >= n */
    for (uint64_t m = n; ; ++m) {
        seq_t val = 0;
        /* Build concatenated number: m, m-1, ..., m-n+1, then 9 */
        for (idx_t i = 0; i < n; ++i) {
            uint64_t cur = m - i;
            int d = num_digits_uint64(cur);
            seq_t pow = pow10_seq(d);
            val = val * pow + (seq_t)cur;
        }
        val = val * 10 + 9;

        if (seq_is_probable_prime(val)) return val;
    }
}
