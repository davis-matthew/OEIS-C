/*
 * A039964  Motzkin numbers A001006 read mod 3.
 * https://oeis.org/A039964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Motzkin number M(n) modulo 3
 * Range:     u8 n <= 2180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: idx_t and seq_t are defined by the build system. */
#include <stdlib.h>

/* Static cache for previously computed values */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Compute the n-th term of A039964 (Motzkin numbers mod 3). */
seq_t a(idx_t n)
{
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* out‑of‑memory: safe failure */
        cache = tmp;

        /* Compute all terms from the current length up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0 || i == 1) {
                cache[i] = (seq_t)1;
            } else {
                seq_t sum = cache[i - 1];
                for (idx_t k = 0; k <= i - 2; ++k) {
                    seq_t prod = (cache[k] * cache[i - 2 - k]) % (seq_t)3;
                    sum += prod;
                }
                cache[i] = sum % (seq_t)3;
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
