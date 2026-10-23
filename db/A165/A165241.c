/*
 * A165241  Triangle T(n,k), 0 <= k <= n, read by rows, given by
 *          [1,1,0,0,0,0,0,0,0,...] DELTA [1,0,1,0,0,0,0,0,0,...] where DELTA
 *          is the operator defined in A084938.
 * https://oeis.org/A165241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle T(n,k) defined by * T(n,k) = 2*T(n-1,k) + 2*T(n-1,k-1)
 *            * - T(n-2,k-1) - T(n-2,k-2) * with T(0,0)=1, T(1,0)=T(1,1)=1 and
 *            T(n,k)=0 if k<0 or k>n. * The sequence a(n) is the linear read
 *            of this triangle by rows
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper macros to allocate/ free arrays of type seq_t in a
 * manner that works for both plain integer types (C) and
 * arbitrary‑precision C++ types (C++17). */
#ifdef __cplusplus
#   define ALLOC_BIG(type, n) new type[(n)]
#   define FREE_BIG(type, ptr) delete[] (ptr)
#else
#   include <stdlib.h>
#   define ALLOC_BIG(type, n) ((type *)malloc(sizeof(type)*(n)))
#   define FREE_BIG(type, ptr) free((ptr))
#endif

seq_t a(idx_t n)
{
    /* Special‑case the first two rows which are trivial. */
    if (n == 0UL) return (seq_t)1;
    if (n == 1UL) return (seq_t)1;
    if (n == 2UL) return (seq_t)1;   /* T(1,1) */

    /* Find the row 'r' such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0;
    idx_t high = n * 2ULL + 2;        /* Safe upper bound */
    idx_t r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Column index within the row */
    unsigned __int128 start = (unsigned __int128)r * (r + 1) / 2;
    idx_t col = n - (idx_t)start;

    /* Prepare the first two rows */
    seq_t *prevprev = ALLOC_BIG(seq_t, 1);
    prevprev[0] = (seq_t)1;             /* Row 0 */

    seq_t *prev = ALLOC_BIG(seq_t, 2);
    prev[0] = prev[1] = (seq_t)1;       /* Row 1 */

    /* Build rows up to r, keeping only the last two rows. */
    for (idx_t i = 2; i <= r; ++i) {
        size_t len = (size_t)(i + 1);   /* length of current row */
        seq_t *cur = ALLOC_BIG(seq_t, len);

        /* k = 0 */
        cur[0] = prev[0] * (seq_t)2;

        /* 1 <= k <= len-2 (interior) */
        for (size_t k = 1; k < len - 1; ++k) {
            seq_t t1 = prev[k]     * (seq_t)2;                /* 2*T(n-1,k) */
            seq_t t2 = prev[k - 1] * (seq_t)2;                /* 2*T(n-1,k-1) */
            seq_t t3 = prevprev[k - 1];                      /* T(n-2,k-1) */
            seq_t t4 = (k >= 2) ? prevprev[k - 2] : (seq_t)0;/* T(n-2,k-2) */
            cur[k] = t1 + t2 - t3 - t4;
        }

        /* k = len-1 (right boundary) */
        /* T(n,n) = 2*T(n-1,n-1) - T(n-2,n-2) */
        cur[len - 1] = prev[len - 2] * (seq_t)2 - prevprev[len - 3];

        /* Advance rows, freeing the oldest one */
        FREE_BIG(seq_t, prevprev);
        prevprev = prev;
        prev = cur;
    }

    /* Result is element 'col' of the last computed row 'prev' */
    seq_t result = prev[(size_t)col];

    /* Clean up the final two rows */
    FREE_BIG(seq_t, prev);
    FREE_BIG(seq_t, prevprev);

    return result;
}
