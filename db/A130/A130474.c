/*
 * A130474  Prime numbers p of the form p=x^2+y^3 such that q =A130475(n)=
 *          abs(x^2-y^3) is prime and smaller than p.
 * https://oeis.org/A130474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   compare_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A130474: primes p = x² + y³ such that q = |x²−y³| is prime
   and q < p.  The function enumerates all candidates up to a dynamic
   limit and caches them.  It works for arbitrary unsigned seq_t
   (uint8_t … __int128) and idx_t = uint64_t. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */
#include "../utils/compare_seq_t.h"  /* compare_seq_t(const void*,const void*) */

static seq_t *generate_terms(uint64_t limit, size_t *out_len)
{
    /* limit is a 64‑bit bound; reduce it to the maximum representable
       value of seq_t. */
    uint64_t max_seq_val = (uint64_t)(seq_t)-1;
    if (limit > max_seq_val) limit = max_seq_val;

    size_t cap = 64;
    seq_t *terms = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!terms) abort();
    size_t len = 0;

    uint64_t y = 1;
    for (;;) {
        uint64_t y3 = y * y * y;
        if (y3 > limit) break;
        uint64_t x = 1;
        for (;;) {
            uint64_t x2 = x * x;
            uint64_t p64 = x2 + y3;
            if (p64 > limit) break;
            uint64_t q64 = (x2 >= y3) ? (x2 - y3) : (y3 - x2);
            if (q64 < 2) { ++x; continue; }

            seq_t p = (seq_t)p64;
            seq_t q = (seq_t)q64;

            if (is_prime_big(p) && is_prime_big(q) && q < p) {
                if (len + 1 > cap) {
                    size_t new_cap = cap * 2;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    terms = tmp;
                    cap = new_cap;
                }
                terms[len++] = p;
            }
            ++x;
        }
        ++y;
    }

    if (len > 1)
        qsort(terms, len, sizeof(seq_t), compare_seq_t);

    seq_t *tmp = (seq_t *)realloc(terms, len * sizeof(seq_t));
    if (!tmp && len > 0) abort();
    terms = tmp;

    *out_len = len;
    return terms;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n == 0) return 0; /* undefined for n = 0 */

    if (cache_len >= n)
        return cache[n - 1];

    /* initial estimate – grows as needed */
    uint64_t limit = (uint64_t)n * 200ULL;
    if (limit < 1000ULL) limit = 1000ULL;

    while (1) {
        size_t len;
        seq_t *terms = generate_terms(limit, &len);

        if (cache) free(cache);
        cache = terms;
        cache_len = len;

        if (cache_len >= n)
            break;

        free(cache);          /* discard produced terms */
        cache = NULL;
        cache_len = 0;
        limit *= 2ULL;        /* increase search range */
    }

    return cache[n - 1];
}
