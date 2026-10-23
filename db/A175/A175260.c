/*
 * A175260  Numbers n with property that n+41, n^2+41 and n^3+41 are all
 *          primes.
 * https://oeis.org/A175260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   mul_mod_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175260: Numbers n such that n+41, n^2+41 and n^3+41 are all primes.
   The function computes the nth term (zero‑based) and memoises the results.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mul_mod_u128.h"

static inline unsigned __int128 pow_mod_u128(unsigned __int128 base,
                                             unsigned __int128 exp,
                                             unsigned __int128 mod)
{
    unsigned __int128 result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1u)
            result = mul_mod_u128(result, base, mod);
        base = mul_mod_u128(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static inline bool is_prime_u128(unsigned __int128 n)
{
    if (n < 2)
        return false;
    /* Small prime trial division */
    static const unsigned int small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    size_t i;
    for (i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        unsigned int p = small_primes[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }

    /* Miller–Rabin bases sufficient for 128‑bit numbers with
       high probability.  The chosen set is small yet effective. */
    static const unsigned int bases[] = {2,3,5,7,11,13,17};
    unsigned __int128 d = n - 1;
    int r = 0;
    while ((d & 1u) == 0u) {
        d >>= 1;
        ++r;
    }

    for (i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        unsigned __int128 a = bases[i];
        if (a % n == 0)
            continue;
        unsigned __int128 x = pow_mod_u128(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        int j;
        for (j = 1; j < r; ++j) {
            x = mul_mod_u128(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Main function: returns the nth term of A175260. */
seq_t a(idx_t n)
{
    /* Memoisation storage for already computed terms. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;
    static unsigned long long next_n = 6;  /* first candidate after 0 */

    if (terms_len == 0) {          /* initialise on first call */
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms[0] = (seq_t)0;       /* a(0) = 0 */
        terms_len = 1;
    }

    /* Produce terms until the requested index is available. */
    while (terms_len <= n) {
        unsigned long long v = next_n;           /* current candidate n */
        unsigned __int128 np1   = (unsigned __int128)v + 41;          /* n+41   */
        if (!is_prime_u128(np1)) { next_n += 6; continue; }

        unsigned __int128 nsq = (unsigned __int128)v * v + 41;       /* n^2+41 */
        if (!is_prime_u128(nsq)) { next_n += 6; continue; }

        unsigned __int128 ncub = (unsigned __int128)v * v * v + 41;   /* n^3+41 */
        if (!is_prime_u128(ncub)) { next_n += 6; continue; }

        /* All three numbers are primes – record the term. */
        if (terms_len == terms_cap) {
            terms_cap <<= 1;
            terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        }
        terms[terms_len++] = (seq_t)v;

        next_n += 6;
    }

    return terms[n];
}
