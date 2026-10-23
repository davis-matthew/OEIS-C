/*
 * A132006  Column 0 and main diagonal (offset) of triangle A132005.
 * https://oeis.org/A132006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n,0) where the triangle T is defined by T(0,0)=1,
 *            T(n,0)=T(n-1,n-1) (n>0), T(n,k)=T(n-1,k-2)+n*T(n-1,k-1) (n>0,
 *            k>1). This function returns a(n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
#  include <cstddef>
#endif

/* Allocate an array of seq_t of length sz.  Handles C and C++ builds. */
static seq_t *alloc_arr(idx_t sz)
{
#if defined(__cplusplus)
    return new seq_t[sz];
#else
    return (seq_t*)calloc(sz, sizeof(seq_t));
#endif
}

/* Free memory allocated by alloc_arr. */
static void free_arr(seq_t *p)
{
#if defined(__cplusplus)
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t size = n + 1;
    seq_t *prev = alloc_arr(size);
    seq_t *curr = alloc_arr(size);
    if (!prev || !curr) {
        free_arr(prev);
        free_arr(curr);
        return (seq_t)0;            /* allocation failed */
    }

    /* initial row n = 0 : T(0,0) = 1 */
    prev[0] = (seq_t)1;

    for (idx_t r = 1; r <= n; ++r) {
        /* T(r,0) = prev[r-1] */
        curr[0] = prev[r - 1];

        /* T(r,1) = r * prev[0] */
        if (r >= 1) {
            curr[1] = (seq_t)r * prev[0];
        }

        /* For k = 2 .. r : T(r,k) = prev[k-2] + r * prev[k-1] */
        for (idx_t k = 2; k <= r; ++k) {
            curr[k] = prev[k - 2] + (seq_t)r * prev[k - 1];
        }

        /* Prepare for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[0];

    free_arr(prev);
    free_arr(curr);
    return result;
}
