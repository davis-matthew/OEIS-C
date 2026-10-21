/*
 * A076944  Least number such that n*k+1 is an n-th power.
 * https://oeis.org/A076944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least n-th power of an integer congruent to 1 modulo n
 *            with k ≥ 1, i.e. a(n) = x^n where x is the smallest integer >1
 *            satisfying x^n ≡ 1 (mod n). The integer a(n) is returned
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute b^e mod m (m > 0) using 128-bit arithmetic to avoid overflow. */
static uint64_t powmod_uint64(uint64_t b, idx_t e, uint64_t m)
{
    uint64_t res = 1 % m;
    uint64_t base = b % m;
    while (e) {
        if (e & 1)
            res = (unsigned __int128)res * base % m;
        base = (unsigned __int128)base * base % m;
        e >>= 1;
    }
    return res;
}

/* Compute base^exp using binary exponentiation, result type seq_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp) {
        if (exp & 1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* handle n == 0 (undefined) by returning 1; but the sequence is defined only for n >= 1 */
    if (n == 0) return 1;

    /* search for smallest x > 1 with x^n ≡ 1 (mod n) */
    idx_t x;
    for (x = 2; x <= n + 1; ++x) {
        if (powmod_uint64((uint64_t)x, n, (uint64_t)n) == 1 % n)
            break;
    }

    /* compute the n-th power, which is the required term a(n) */
    return pow_seq((seq_t)x, n);
}
