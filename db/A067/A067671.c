/*
 * A067671  The prime factors of n are also prime factors of the decimal
 *          encoding (A067599) of the prime factorization of n.
 * https://oeis.org/A067671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculates OEIS A067671: a(n) is the n‑th integer for which every prime
 * factor of the integer also divides the decimal encoding of its prime
 * factorization (A067599).  The routine works by incrementally
 * testing successive integers, caching all terms found so far.  For each
 * candidate number it factorizes the integer (using factor_u64 from
 * utils/factor_u64.h) and verifies that each prime factor p satisfies
 * p | E(n), where E(n) is the concatenation of the prime factors and
 * exponents in decimal.  E(n) mod p is computed digit by digit, so no
 * large integer arithmetic is required.  The function is memoised in
 * static cache to answer queries in any order in O(n) time.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static seq_t *cache = NULL;      /* cached sequence values   */
static size_t  cache_len = 0;    /* number of cached terms   */
static size_t  cache_cap = 0;    /* cache capacity           */
static uint64_t next_scan = 2;   /* next integer to test     */

/* Resize the cache so that it can hold at least 'needed' elements. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = newp;
    cache_cap = new_cap;
}

/* Return 1 iff every prime factor of 'n' divides the decimal encoding
 * of its prime factorization; 0 otherwise. */
static int satisfies_property(uint64_t n)
{
    uint64_t fac[64], exp[64];
    size_t cnt;
    factor_u64(n, fac, exp, &cnt);
    if (cnt == 0) return 0;  /* n == 1 */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = fac[i];
        uint64_t mod = 0;

        /* Process all prime–exponent pairs to build the encoding digit by digit */
        for (size_t j = 0; j < cnt; ++j) {
            /* prime part */
            uint64_t x = fac[j];
            uint8_t digits[32];
            int dlen;
            if (x == 0) { digits[0] = 0; dlen = 1; }
            else {
                dlen = 0;
                while (x) {
                    digits[dlen++] = x % 10;
                    x /= 10;
                }
            }
            for (int k = dlen - 1; k >= 0; --k)
                mod = (((unsigned __int128)mod) * 10 + digits[k]) % p;

            /* exponent part */
            x = exp[j];
            if (x == 0) { digits[0] = 0; dlen = 1; }
            else {
                dlen = 0;
                while (x) {
                    digits[dlen++] = x % 10;
                    x /= 10;
                }
            }
            for (int k = dlen - 1; k >= 0; --k)
                mod = (((unsigned __int128)mod) * 10 + digits[k]) % p;
        }

        if (mod != 0) return 0;  /* prime p does not divide the encoding */
    }
    return 1;  /* all primes divide the encoding */
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* unused, but safe */

    /* Generate terms until we have at least n terms cached. */
    while (cache_len < (size_t)n) {
        if (satisfies_property(next_scan)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_scan;
        }
        ++next_scan;
    }

    return cache[n - 1];
}
