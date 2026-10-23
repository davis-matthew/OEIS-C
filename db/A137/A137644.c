/*
 * A137644  a(n) = Sum_{k=0..n} C(n+k,k)*C(n+k,n-k).
 * https://oeis.org/A137644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n+k,k) * C(n+k,n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts lattice paths from (0,0) to (n,n) using steps
   (1,0),(0,1),(1,1),(0,2).  The following implementation uses a
   dynamic‑programming recurrence that needs only two rows of the
   Pascal‑triangular–like table, thus running in O(n^2) time and O(n)
   memory.  The code is valid C11 and C++17. */

#include <stddef.h>
#include <stdlib.h>

/* Allocation helpers that work in both C and C++ */
#ifdef __cplusplus
# define ALLOC_SEQ_T(t, cnt) new t[(cnt)]
# define FREE_SEQ_T(ptr) delete[] (ptr)
#else
# define ALLOC_SEQ_T(t, cnt) ((t*)malloc((cnt)*sizeof(t)))
# define FREE_SEQ_T(ptr) free(ptr)
#endif

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    size_t N = (size_t)n;          /* array dimension */

    seq_t *prev = ALLOC_SEQ_T(seq_t, N + 1);
    seq_t *curr = ALLOC_SEQ_T(seq_t, N + 1);
    if (!prev || !curr) {          /* allocation failure – unlikely */
        return (seq_t)0;
    }

    /* Row i = 0 : T(0,j) = T(0,j-1) + T(0,j-2) (j >= 2), T(0,0)=1 */
    prev[0] = 1;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t val = prev[(size_t)j - 1];
        if (j >= 2)
            val += prev[(size_t)j - 2];
        prev[(size_t)j] = val;
    }

    /* DP for rows i = 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        curr[0] = 1;                     /* T(i,0) = 1 (only horizontal steps) */
        for (idx_t j = 1; j <= n; ++j) {
            seq_t val = prev[(size_t)j];             /* T(i-1,j) */
            val += curr[(size_t)j - 1];             /* T(i,j-1) */
            val += prev[(size_t)j - 1];             /* T(i-1,j-1) */
            if (j >= 2)
                val += curr[(size_t)j - 2];         /* T(i,j-2) */
            curr[(size_t)j] = val;
        }
        /* swap rows for next iteration */
        seq_t *tmp  = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[(size_t)n];

    FREE_SEQ_T(prev);
    FREE_SEQ_T(curr);
    return result;
}
