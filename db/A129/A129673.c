/*
 * A129673  Right truncatable primes in base 7 (written in decimal form).
 * https://oeis.org/A129673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function returns the n‑th term (1‑based) of OEIS sequence A129673
    – right truncatable primes in base 7 (decimal representation).
    The value is computed by exploring all base‑7 primes and storing them
    in increasing decimal order.  For n greater than the number of
    computable terms (depending on the size of seq_t) this function
    returns 0. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Comparison function for qsort (works for any seq_t) */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

/* Return the n‑th right truncatable prime in base 7 (1‑based). */
seq_t a(idx_t n)
{
    /* Static cache of sorted terms, lazily built on first call. */
    static seq_t *cache          = NULL;
    static size_t cache_len      = 0;
    static bool initialized      = false;

    /* Build the cache once. */
    if (!initialized) {
        /* Use a dynamic vector to accumulate primes. */
        size_t cap = 16;
        size_t sz  = 0;
        seq_t *vec = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!vec) return 0; /* allocation failure */

        /* Single‑digit base‑7 primes: 2, 3, 5. */
        vec[sz++] = (seq_t)2;
        vec[sz++] = (seq_t)3;
        vec[sz++] = (seq_t)5;

        /* Breadth‑first expansion: for each prime q, try all digits d=0..6. */
        for (size_t i = 0; i < sz; ++i) {
            seq_t q = vec[i];
            for (int d = 0; d <= 6; ++d) {
                seq_t cand = q * (seq_t)7 + (seq_t)d;
                /* Only test if cand fits in seq_t; if not, the cast will
                   wrap, so the primality test will fail – hence we ignore
                   over‑overflowing candidates. */
                if (is_prime_big(cand)) {
                    if (sz == cap) {
                        cap *= 2;
                        seq_t *tmp = (seq_t *)realloc(vec, cap * sizeof(seq_t));
                        if (!tmp) { free(vec); return 0; }
                        vec = tmp;
                    }
                    vec[sz++] = cand;
                }
            }
        }

        /* Sort the collected primes in increasing order. */
        qsort(vec, sz, sizeof(seq_t), cmp_seq_t);

        /* Allocate and copy into the cache. */
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) { free(vec); return 0; }
        for (size_t i = 0; i < sz; ++i) cache[i] = vec[i];
        free(vec);
        cache_len = sz;
        initialized = true;
    }

    /* Return the requested term, or 0 if n is out of bounds. */
    if (n == 0) return 0;
    if (n <= (idx_t)cache_len) return cache[(size_t)(n - 1)];
    return 0;
}
