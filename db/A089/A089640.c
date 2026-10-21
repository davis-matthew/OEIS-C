/*
 * A089640  Enumeration of partial sums of 1 + [1,2] + [2,3] + [1,2] + [2,3] +
 *          ...
 * https://oeis.org/A089640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A089640 – Enumeration of partial sums of 1 + [1,2] + [2,3] + [1,2] + [2,3] + …
    Formula:  a(n) = a(n‑3) + 2*a(n‑4) + a(n‑5)   (for n > 5)
    Initial values (index 1 = first term):  a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=3   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* index 0 is outside the OEIS range */
        return (seq_t)0;

    static seq_t *cache = NULL;       /* cache[a(1)] … cache[a(n)] */
    static idx_t cache_size = 0;      /* allocated size of cache (>=1) */
    static idx_t computed = 0;        /* highest n for which values are known */

    /* --------- grow cache if needed ------------------------------------- */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        if (new_size < 6) new_size = 6;          /* at least 5 base entries */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                       /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* --------- initialise the five base values --------------------------- */
    /* they can be overwritten safely; they are constants */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)1;
    cache[4] = (seq_t)1;
    cache[5] = (seq_t)3;

    /* --------- compute extra values using the recurrence --------------- */
    if (computed < 5)                     /* first time, base values set */
        computed = 5;

    if (n > computed) {
        idx_t start = computed + 1;
        if (start < 6) start = 6;          /* recurrence starts at a(6) */
        for (idx_t i = start; i <= n; ++i) {
            /* a(i) = a(i‑3) + 2*a(i‑4) + a(i‑5) */
            cache[i] = cache[i-3] + (cache[i-4] << 1) + cache[i-5];
        }
        computed = n;
    }

    return cache[n];
}
