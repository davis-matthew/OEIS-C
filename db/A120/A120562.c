/*
 * A120562  Sum of binomial coefficients binomial(i+j, i) modulo 2 over all
 *          pairs (i,j) of positive integers satisfying 3i+j=n.
 * https://oeis.org/A120562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1, a(2n)=a(n), a(2n+1)=a(n)+a(n-1) (for n≥1)
 * Range:     u8 n <= 7158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Resize cache so that it can index up to n */
static void ensure_cache(idx_t n) {
    if (n + 1 <= cache_size)
        return;
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – abort gracefully */
        exit(1);
    }
    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n) {
    /* Handle small indices explicitly */
    if (n == 0 || n == 1) {
        return (seq_t)1;
    }

    /* Make sure cache is big enough and base values are set */
    ensure_cache(n);
    if (cache[0] == 0)
        cache[0] = (seq_t)1;       /* a(0) */
    if (cache[1] == 0)
        cache[1] = (seq_t)1;       /* a(1) */

    /* Compute values up to n (repeating previous work on subsequent calls is fine) */
    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1) == 0) {             /* i is even: a(2k) = a(k) */
            cache[i] = cache[i >> 1];
        } else {                         /* i is odd: a(2k+1) = a(k) + a(k-1) */
            idx_t k = i >> 1;
            seq_t v1 = cache[k];
            seq_t v0 = (k == 0) ? (seq_t)0 : cache[k - 1];
            cache[i] = v1 + v0;
        }
    }
    return cache[n];
}
