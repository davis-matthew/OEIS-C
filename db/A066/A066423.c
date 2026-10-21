/*
 * A066423  Composite numbers n such that the product of proper divisors of
 *          the n does not equal n.
 * https://oeis.org/A066423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth composite number n whose number of divisors d(n)
 *            ≠ 4 (i.e., n is not a product of two distinct primes nor a cube
 *            of a prime)
 * Range:     u8 n <= 121, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns a(idx_t n) with 1‑based indexing. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache     = NULL;   /* stored terms */
    static size_t cache_cap = 0;      /* capacity of cache */
    static size_t cache_len = 0;      /* number of filled entries */
    static uint64_t cand = 4;         /* current integer to test */

    /* Keep generating terms until we have at least 'n' of them. */
    while (cache_len < n) {
        uint64_t x   = cand;
        uint64_t tmp = x;
        size_t   d   = 1;              /* current divisor count */

        /* trial factorization to compute d(x) */
        for (uint64_t p = 2; p * p <= tmp; ++p) {
            if (tmp % p == 0) {
                size_t exp = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++exp;
                }
                d *= (exp + 1);
                if (d > 4) break;      /* early exit if already >4 */
            }
        }
        if (tmp > 1)               /* remaining prime factor */
            d *= 2;

        /* n is composite iff d > 2.  Include it if d != 4. */
        if (d > 2 && d != 4) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp_cache) abort();
                cache = tmp_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }

        ++cand;   /* next integer to test */
    }

    return cache[n - 1];
}
