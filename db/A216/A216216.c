/*
 * A216216  Square array T, read by antidiagonals: T(n,k) = 0 if n-k>=3 or if
 *          k-n>=3, T(2,0) = T(1,0) = T(0,0) = T(0,1) = T(0,2) = 1, T(n,k) =
 *          T(n-1,k) + T(n,k-1).
 * https://oeis.org/A216216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry T(i,j) of square array T read by
 *            antidiagonals. The array is defined by
 *            T(0,0)=T(0,1)=T(0,2)=T(1,0)=T(2,0)=1; T(n,k)=0 if |n-k|>=3;
 *            otherwise T(n,k)=T(n-1,k)+T(n,k-1). The sequence a(n) is
 *            obtained by traversing T diagonally: a(0)=T(0,0), a(1)=T(0,1),
 *            a(2)=T(1,0), a(3)=T(0,2), …...
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* global cache for the table */
static seq_t *T = NULL;
static idx_t T_size = 0;           /* current dimension of the square table */

/* recompute the full table up to dimension newsize */
static void recompute_table(idx_t newsize)
{
    idx_t i, j;
    /* allocate new table */
    seq_t *newT = (seq_t *)malloc(newsize * newsize * sizeof(seq_t));
    if (!newT) return;  /* out‑of‑memory: caller will return 0 */

    /* initialise to zero */
    for (idx_t k = 0; k < newsize * newsize; ++k) newT[k] = 0;

    /* base values (only for indices that exist) */
    if (newsize > 0)      newT[0]                           = 1;   /* T(0,0) */
    if (newsize > 1)      newT[1]                           = 1;   /* T(0,1) */
    if (newsize > 2)      newT[2]                           = 1;   /* T(0,2) */
    if (newsize > 1)      newT[newsize]                     = 1;   /* T(1,0) */
    if (newsize > 2)      newT[2 * newsize]                 = 1;   /* T(2,0) */

    /* compute all remaining entries by the recurrence */
    for (i = 0; i < newsize; ++i)
    {
        for (j = 0; j < newsize; ++j)
        {
            /* already handled in the base part ? */
            if (i == 0 && j <= 2) continue;
            if (i <= 2 && j == 0) continue;

            /* out of band? */
            if (i >= j)
            {
                if ((i - j) >= 3) { newT[i * newsize + j] = 0; continue; }
            }
            else
            {
                if ((j - i) >= 3) { newT[i * newsize + j] = 0; continue; }
            }

            /* inside band: recurrence */
            seq_t val = 0;
            if (i > 0)             val += newT[(i - 1) * newsize + j];
            if (j > 0)             val += newT[i * newsize + (j - 1)];
            newT[i * newsize + j] = val;
        }
    }

    /* replace old table (if any) */
    if (T) free(T);
    T = newT;
    T_size = newsize;
}

/* return the diagonal index d for given n: n = d(d+1)/2 + i */
static idx_t diag_of(idx_t n)
{
    idx_t d = 0;
    while ( (d + 1) * (d + 2) / 2 <= n ) ++d;
    return d;
}

/* sequence entry */
seq_t a(idx_t n)
{
    idx_t d = diag_of(n);          /* diagonal containing the desired entry */
    idx_t need = d + 1;            /* table dimension required */

    if (need > T_size)                     /* enlarge table if necessary */
        recompute_table(need);

    idx_t base = d * (d + 1) / 2;
    idx_t i = n - base;                  /* row index */
    idx_t j = d - i;                     /* column index */

    return T[i * T_size + j];
}
