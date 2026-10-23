/*
 * A025264  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+ a(n-1)*a(1) for n >= 4,
 *          starting 2,1,1.
 * https://oeis.org/A025264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025264: a(n) = a(1)*a(n-1)+a(2)*a(n-2)+...+a(n-1)*a(1) for n>=4,
   with a(1)=2, a(2)=1, a(3)=1.  The array is built on demand and memoised. */
#include <stdlib.h>
#include <stdint.h>

/* Note: seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* Static cache holding computed terms.  tbl[0] = a(1). */
    static seq_t *tbl = NULL;   /* allocated size in elements */
    static size_t cap = 0;      /* current allocated size */
    static idx_t max_n = 0;     /* number of terms already computed */

    if (n == 0)          /* undefined for n=0, return 0 */
        return 0;

    /* Return from cache if already computed. */
    if (n <= (idx_t)max_n)
        return tbl[n - 1];

    /* Grow cache to at least n elements. */
    size_t new_cap = (cap > 0) ? cap : 1;
    while (new_cap < (size_t)n)
        new_cap <<= 1;               /* double until enough */

    seq_t *new_tbl = (seq_t *)realloc(tbl, new_cap * sizeof(seq_t));
    if (new_tbl == NULL)            /* memory allocation failed */
        return 0;
    tbl = new_tbl;
    cap = new_cap;

    /* Compute terms from max_n to n-1 (zero-based indices). */
    for (size_t i = (size_t)max_n; i < (size_t)n; ++i)
    {
        if (i == 0)          /* a(1) */
            tbl[i] = (seq_t)2;
        else if (i == 1 || i == 2)  /* a(2) = a(3) = 1 */
            tbl[i] = (seq_t)1;
        else
        {
            seq_t sum = (seq_t)0;
            /* sum over k = 1..i+1 of a(k)*a((i+1)-k) */
            /* zero-based: tbl[k-1] * tbl[i-k] */
            for (idx_t k = 1; k <= (idx_t)i; ++k)
            {
                seq_t ak = tbl[k - 1];
                seq_t aj = tbl[i - k];
                /* use 128-bit intermediate to avoid overflow when seq_t is 64-bit */
                seq_t prod = (seq_t)(((__uint128_t)ak) * ((__uint128_t)aj));
                sum += prod;
            }
            tbl[i] = sum;
        }
    }

    max_n = (idx_t)n;
    return tbl[n - 1];
}
