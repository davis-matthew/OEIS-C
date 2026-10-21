/*
 * A229667  Number of defective 4-colorings of an nX3 0..3 array connected
 *          horizontally, antidiagonally and vertically with exactly two
 *          mistakes, and colors introduced in row-major 0..3 order.
 * https://oeis.org/A229667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) - 171*a(n-2) + 679*a(n-3) - 1368*a(n-4) * +
 *            1344*a(n-5) - 512*a(n-6) for n > 7 * Initial values: a(1)=1,
 *            a(2)=61, a(3)=1555, a(4)=19805, * a(5)=194575, a(6)=1673561,
 *            a(7)=13271403 * The sequence is computed iteratively with
 *            memoisation
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* 0 is outside the sequence */

    /* Ensure we have at least INIT_N elements pre‑initialised */
    const idx_t INIT_N = 7;
    static seq_t *cache = NULL;                /* array of computed terms   */
    static idx_t cache_size = 0;               /* allocated size, in elements */
    static idx_t computed = 0;                 /* highest index that has been computed (1‑based) */

    /* Allocate (or grow) storage so that it can hold up to max(n, INIT_N) entries */
    idx_t need = (n > INIT_N) ? n : INIT_N;
    if (cache_size < need) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * need);
        if (!tmp) abort();                     /* allocation failure */
        /* zero‑initialise newly added entries */
        for (idx_t i = cache_size; i < need; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = need;
    }

    /* On first use, or if we have not yet computed the first seven values,
       fill them from the known table */
    if (computed < 7) {
        static const seq_t init[7] = {
            (seq_t)1,        /* a(1) */
            (seq_t)61,       /* a(2) */
            (seq_t)1555,     /* a(3) */
            (seq_t)19805,    /* a(4) */
            (seq_t)194575,   /* a(5) */
            (seq_t)1673561,  /* a(6) */
            (seq_t)13271403  /* a(7) */
        };
        idx_t start = computed;                 /* next index to fill (0‑based) */
        for (idx_t i = start; i < 7; ++i) {
            cache[i] = init[i];
        }
        computed = 7;
    }

    /* If the request is already satisfied, return the cached value */
    if (n <= computed) return cache[n - 1];

    /* Compute missing terms using the 6‑term linear recurrence */
    for (idx_t k = computed + 1; k <= n; ++k) {
        /* k is the 1‑based index of the term we are computing
           cache[k-1] will store a(k) */
        seq_t term = cache[k - 2];          /* a(k-1)  */
        term *= (seq_t)21;                 /* 21*a(k-1) */

        seq_t second = cache[k - 3];        /* a(k-2)  */
        second *= (seq_t)171;               /* 171*a(k-2) */

        seq_t third = cache[k - 4];         /* a(k-3)  */
        third *= (seq_t)679;                /* 679*a(k-3) */

        seq_t fourth = cache[k - 5];        /* a(k-4)  */
        fourth *= (seq_t)1368;              /* 1368*a(k-4) */

        seq_t fifth = cache[k - 6];         /* a(k-5)  */
        fifth *= (seq_t)1344;               /* 1344*a(k-5) */

        seq_t sixth = cache[k - 7];         /* a(k-6)  */
        sixth *= (seq_t)512;                /* 512*a(k-6) */

        /* perform the addition/subtraction in a safe order */
        seq_t pos = term + third + fifth;
        seq_t neg = second + fourth + sixth;
        cache[k - 1] = pos - neg;
    }

    computed = n;
    return cache[n - 1];
}
