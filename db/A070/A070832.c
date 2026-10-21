/*
 * A070832  a(n) = Sum_{k=0..n} binomial(8*n,8*k).
 * https://oeis.org/A070832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(8*n, 8*k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 136*a(n-1) + 32880*a(n-2) - 552704*a(n-3) - 65536*a(n-4) for n ≥ 5 */
/* Initial values: a(0)=1, a(1)=2, a(2)=12872, a(3)=1470944, a(4)=622116992 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t computed = (idx_t)-1; /* none computed yet */

    if (n == 0) {
        /* Ensure cache[0] */
        if (cache == NULL) {
            cache = (seq_t *)malloc(sizeof(seq_t) * 1);
            cache[0] = (seq_t)1;
        } else if (computed < 0) {
            cache[0] = (seq_t)1;
        }
        computed = 0;
        return cache[0];
    }

    /* Ensure cache can hold up to n */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        cache[0] = (seq_t)1;
        computed = 0;
    } else if ((idx_t)cache[0] == (idx_t)-1) { /* guard earlier init */
        /* nothing */
        ;
    } else if ((idx_t)cache[0] == (idx_t)-1) {
        /* nothing */
        ;
    }
    size_t needed = (size_t)n + 1;
    if (needed > (size_t)computed + 1) {
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    }

    /* Compute from computed+1 up to n */
    if (computed < n) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)2;
            } else if (i == 2) {
                cache[2] = (seq_t)12872;
            } else if (i == 3) {
                cache[3] = (seq_t)1470944;
            } else if (i == 4) {
                cache[4] = (seq_t)622116992;
            } else {
                /* recurrence with 64‑bit ints, works for any seq_t underlying type */
                cache[i] =
                    (seq_t)136 * cache[i-1] +
                    (seq_t)32880 * cache[i-2] -
                    (seq_t)552704 * cache[i-3] -
                    (seq_t)65536 * cache[i-4];
            }
        }
        computed = n;
    }

    return cache[n];
}
