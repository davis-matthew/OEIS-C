/*
 * A216210  Square array T read by antidiagonals: T(n,k) = 0 if n-k>=4 or if
 *          k-n>=4, T(3,0) = T(2,0) = TT(1,0) = T(0,0) = T(0,1) = T(0,2) =
 *          T(0,3) = 1, T(n,k) = T(n-1,k) + T(n,k-1).
 * https://oeis.org/A216210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from the banded Pascal‑like triangle T. * T(n,k) =
 *            0 if |n-k| >= 4 * T(n,0) = 1 for n <= 3, otherwise 0 * T(n,k) =
 *            T(n-1,k) + T(n,k-1) for the remaining cells. * a(n) = T(i,j)
 *            where i+j = d, * d*(d+1)/2 <= n < (d+1)*(d+2)/2, * i = n -
 *            d*(d+1)/2, j = d - i
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: allocate an array of seq_t of given size.  Works for both
 * C (malloc) and C++ (new). */
static seq_t* alloc_seq_array(idx_t sz)
{
#ifdef __cplusplus
    return new seq_t[sz];
#else
    return (seq_t*)calloc(sz, sizeof(seq_t));
#endif
}

/* Helper: free an array obtained from alloc_seq_array. */
static void free_seq_array(seq_t* p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    /* --- 1. Determine the diagonal d and offsets i, j --- */
    idx_t d = 0;
    /* find first d with pref > n, where pref = d*(d+1)/2 */
    while (1) {
        unsigned __int128 pref128 = (unsigned __int128)d * (d + 1) / 2;
        if (pref128 > n) break;
        ++d;
    }
    d--;                                  /* now d*(d+1)/2 <= n */
    unsigned __int128 pref128 = (unsigned __int128)d * (d + 1) / 2;
    idx_t pref = (idx_t)pref128;           /* pref <= n */
    idx_t offset = n - pref;              /* 0 <= offset <= d */
    idx_t i = offset;                     /* row index */
    idx_t j = d - offset;                 /* column index */

    /* --- 2. DP over the required rows --- */
    seq_t* prev = alloc_seq_array(d + 1);
    seq_t* curr = alloc_seq_array(d + 1);
    if (!prev || !curr) {                 /* allocation failure */
        free_seq_array(prev);
        free_seq_array(curr);
        return (seq_t)0;
    }

    seq_t result = 0;

    for (idx_t row = 0; row <= i; ++row) {
        /* column 0 */
        curr[0] = (row <= 3 ? (seq_t)1 : (seq_t)0);

        /* columns 1..d */
        for (idx_t col = 1; col <= d; ++col) {
            if ((row > col && (row - col) >= 4) ||
                (col > row && (col - row) >= 4)) {
                curr[col] = (seq_t)0;
            } else {
                curr[col] = prev[col] + curr[col - 1];
            }
        }

        if (row == i) {
            result = curr[j];
        }

        /* swap previous and current for next iteration */
        seq_t* tmp = prev;
        prev = curr;
        curr = tmp;
    }

    free_seq_array(prev);
    free_seq_array(curr);
    return result;
}
