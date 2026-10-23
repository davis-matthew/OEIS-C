/*
 * A020882  Ordered hypotenuses (with multiplicity) of primitive Pythagorean
 *          triangles.
 * https://oeis.org/A020882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 7093, u32 n <= 9699, u64 n <= 9699,
 *            u128 n <= 9699, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = sqrt( (A120681(n)^2 + A120682(n)^2)/2 )
 * The implementation enumerates all primitive Pythagorean triples
 * (a,b,c) with a>b>0, gcd(a,b)=1 and not both odd.
 * For each triple it records the hypotenuse c = a² + b².
 * Duplicates (different triples producing the same c) are kept.
 * The nth smallest c with multiplicity is returned.
 */
 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* comparison routine for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t v1 = *(const seq_t *)p1;
    seq_t v2 = *(const seq_t *)p2;
    if (v1 < v2) return -1;
    if (v1 > v2) return  1;
    return 0;
}

/* routine that grows a dynamic array when it becomes full */
static void grow_seq(seq_t **arr, size_t *cap, size_t *cnt)
{
    size_t old_cap = *cap;
    *cap <<= 1;                     /* double the capacity */
    seq_t *tmp;
#if defined(__cplusplus)
    tmp = new seq_t[*cap];
    for (size_t i = 0; i < *cnt; ++i) tmp[i] = (*arr)[i];
    delete [] *arr;
#else
    tmp = (seq_t *)realloc(*arr, *cap * sizeof(seq_t));
#endif
    *arr = tmp;
}

/* main sequence routine */
seq_t a(idx_t n)
{
    /* a reasonable upper bound on a(n) is 2*Pi*n ≈ 6.3*n.
       We take a conservative multiplier of 8 and add a safety margin. */
    idx_t bound_idx = n * 8ULL + 100ULL;         /* bound on the index needed */
    if (bound_idx == 0) bound_idx = 1;          /* avoid overflow when n is huge */

    while (true)                               /* we may increase the bound if needed */
    {
        /* The bound for the hypotenuse value in the current search */
        seq_t bound_seq = (seq_t)bound_idx;

        size_t cap = 256;                       /* initial allocation, will grow as needed */
        size_t cnt = 0;
        seq_t *cvals;

#if defined(__cplusplus)
        cvals = new seq_t[cap];
#else
        cvals = (seq_t *)malloc(cap * sizeof(seq_t));
#endif
        if (!cvals) return 0;                   /* allocation failure  */

        /* Enumerate all primitive triples using Euclid’s parameterisation:
           a = m² - n², b = 2mn, c = m² + n²  with m>n>0, gcd(m,n)=1,
           and m-n odd. */
        for (idx_t m = 2; ; ++m)
        {
            seq_t m_sq = (seq_t)m * (seq_t)m;
            if (m_sq > bound_seq) break;       /* m² already exceeds the bound */

            for (idx_t k = 1; k < m; ++k)
            {
                seq_t k_sq = (seq_t)k * (seq_t)k;
                seq_t c = m_sq + k_sq;
                if (c > bound_seq) break;       /* further k only increase c */

                if (((m - k) & 1U) && (gcd((seq_t)m, (seq_t)k) == 1))
                {
                    if (cnt == cap) grow_seq(&cvals, &cap, &cnt);
                    cvals[cnt++] = c;
                }
            }
        }

        if (cnt >= n)                            /* we have enough values */
        {
            qsort(cvals, cnt, sizeof(seq_t), cmp_seq_t);
            seq_t result = cvals[n - 1];
#if defined(__cplusplus)
            delete [] cvals;
#else
            free(cvals);
#endif
            return result;
        }

        /* Not enough values – enlarge the bound and retry. */
#if defined(__cplusplus)
        delete [] cvals;
#else
        free(cvals);
#endif
        if (bound_idx > ((idx_t)(~(seq_t)0)) / 2)   /* prevent overflow on next shift */
            bound_idx = (idx_t)(~(seq_t)0);
        else
            bound_idx <<= 1;                       /* double the bound index */
    }

    return 0; /* never reached */
}
