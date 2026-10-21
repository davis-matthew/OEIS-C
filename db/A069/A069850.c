/*
 * A069850  Number of distinct primes obtained by inserting a 9 at all
 *          possible places in n. This includes prefixing as well as
 *          suffixing.
 * https://oeis.org/A069850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A069850: Number of distinct primes obtained by inserting a 9
 *  into the decimal representation of n (including prefixing and suffixing). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*--------------------------------------------------------------------
 *  Miller–Rabin primality test for 64‑bit integers
 *--------------------------------------------------------------------*/

static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) res = mul_mod_uint64(res, base, mod);
        base = mul_mod_uint64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                    19ULL,23ULL,29ULL,31ULL,37ULL,0ULL};
    for (int i = 0; small[i]; ++i) {
        if (n % small[i] == 0ULL) return n == small[i];
    }

    /* n‑1 = d · 2^s  */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit numbers */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL, 0ULL};
    for (int i = 0; bases[i]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/*--------------------------------------------------------------------
 *  Main sequence function
 *--------------------------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* below offset – any value accepted */

    /* Count decimal digits of n */
    idx_t tmp = n;
    size_t digits = 0;
    do { ++digits; tmp /= 10; } while (tmp);

    /* Avoid overflow – keep to numbers that fit in 64 bits */
    if (digits > 18) return (seq_t)0;   /* too many digits for this implementation */

    /* Powers of 10 up to digits+1, all fit in 64 bits */
    uint64_t pow10[20];
    pow10[0] = 1ULL;
    for (size_t i = 1; i <= digits + 1; ++i) pow10[i] = pow10[i - 1] * 10ULL;

    /* Store distinct prime candidates (max digits+1 candidates) */
    uint64_t primes[22];
    size_t count = 0;

    for (size_t i = 0; i <= digits; ++i) {
        /* split n into high / low */
        uint64_t high = n / pow10[digits - i];
        uint64_t low  = n % pow10[digits - i];

        /* build candidate: high * 10^{L-i+1} + 9 * 10^{L-i} + low */
        unsigned __int128 cand128 =
            (unsigned __int128)high * pow10[digits - i + 1] +
            (unsigned __int128)9ULL * pow10[digits - i] +
            low;

        if (cand128 > UINT64_MAX) continue;           /* skip – too large for this test */

        uint64_t cand = (uint64_t)cand128;
        if (!is_prime_uint64(cand)) continue;

        /* check distinctness */
        bool seen = false;
        for (size_t j = 0; j < count; ++j)
            if (primes[j] == cand) { seen = true; break; }
        if (!seen) primes[count++] = cand;
    }
    return (seq_t)count;
}
