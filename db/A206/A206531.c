/*
 * A206531  a(n) = (2*(n+1)*(2*n+1)-1)*a(n-1) + 2*n*(2*n-1)*a(n-2), a(0)=0,
 *          a(1)=2.
 * https://oeis.org/A206531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*(n+1)*(2*n+1)-1)*a(n-1) + 2*n*(2*n-1)*a(n-2); a(0)=0,
 *            a(1)=2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0..n] stores computed terms */
static size_t cap = 0;        /* current capacity of cache */
static idx_t computed = 0;    /* highest index that has been computed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;

    /* Ensure we have enough space in the cache */
    if (cap <= (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) abort();  /* out of memory – terminate */
        cache = tmp;
        cap = (size_t)n + 1;
    }

    /* Initialise base values if this is the first real computation. */
    if (computed < 1) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        computed = 1;
    }

    /* Compute terms from (computed+1) up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t two = (seq_t)2;
        seq_t kn1 = (seq_t)k + (seq_t)1;
        seq_t two_k = two * (seq_t)k;
        seq_t coeff1 = two * kn1 * (two_k + (seq_t)1) - (seq_t)1;
        seq_t coeff2 = two * (seq_t)k * (two_k - (seq_t)1);
        cache[k] = coeff1 * cache[k - 1] + coeff2 * cache[k - 2];
    }

    computed = n;
    return cache[n];
}
