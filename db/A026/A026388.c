/*
 * A026388  a(n) is the number of integer strings s(0),...,s(n) counted by
 *          array T in A026386 that have s(n)=2; also a(n) = T(2n,n-1).
 * https://oeis.org/A026388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=5,
   a(n) = ((12*n^2-2*n+1)*a(n-1) - 5*(2*n+1)*(n-2)*a(n-2)) / ((n+1)*(2*n-1))
   for n >= 3 (offset 1) */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache of already computed values */
    static seq_t *cache = NULL;         /* array of a values */
    static idx_t cache_size = 0;        /* allocated array size */
    static idx_t cached_cnt = 0;        /* number of values stored */

    if (n == 0) return 0; /* undefined, but avoid UB */

    /* initialise cache with first two terms */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) { /* allocation failure – abort */
            abort();
        }
        cache[0] = (seq_t)1;          /* a(1) */
        cache[1] = (seq_t)5;          /* a(2) */
        cached_cnt = 2;
    }

    /* grow cache if needed */
    while (cached_cnt < n) {
        idx_t k = cached_cnt + 1; /* actual n value to compute next */
        if (cache_size == 0 || cached_cnt == cache_size) {
            size_t new_size = cache_size * 2;
            if (new_size == 0) new_size = 4;
            seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
            if (tmp == NULL) { abort(); }
            cache = tmp;
            cache_size = new_size;
        }

        /* a(k-2) and a(k-1) are already in cache */
        seq_t a_prev = cache[k - 2];           /* a(k-1) */
        seq_t a_prev2 = cache[k - 3];          /* a(k-2) */

        /* compute coefficients using only seq_t arithmetic */
        seq_t coeff1 = (seq_t)12 * (seq_t)k * (seq_t)k
                       - (seq_t)2 * (seq_t)k
                       + (seq_t)1;                       /* 12*k^2-2*k+1 */
        seq_t term1 = coeff1 * a_prev;

        seq_t coeff2 = (seq_t)5 * (seq_t)(2 * k + 1) * (seq_t)(k - 2); /* 5*(2*k+1)*(k-2) */
        seq_t term2 = coeff2 * a_prev2;

        seq_t numerator = term1 - term2;
        seq_t denominator = (seq_t)(k + 1) * (seq_t)(2 * k - 1);

        seq_t a_cur = numerator / denominator;
        cache[cached_cnt] = a_cur;
        cached_cnt++;
    }

    return cache[n - 1];
}
