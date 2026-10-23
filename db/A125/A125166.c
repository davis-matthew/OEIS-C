/*
 * A125166  Triangle R(n,k), companion to A125165, left column n^3.
 * https://oeis.org/A125166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 108, u32 n <= 506, u64 n <= 2042,
 *            u128 n <= 8311, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A125166 – Triangle R(n,k) flattened row‑by‑row
 *
 *  R(n,0) = (n+1)^3
 *  R(n,k) = R(n-1,k-1) + R(n-1,k)   for k >= 1
 *
 *  A(n) is the n‑th element when the triangle is read by rows:
 *  A(0)=R(0,0), A(1)=R(1,0), A(2)=R(1,1), A(3)=R(2,0), ...
 *
 *  This implementation computes A(n) by determining the row r
 *  containing the index n, then building the rows of the triangle
 *  up to r using a simple Pascal recursion.
 *
 *  The routine works in O(n) time with O(r) memory (r ≈ √(2n)),
 *  easily fast enough for n up to 10 000 and far larger.
 */

#include <stdlib.h>
#include <stdint.h>

/* helper: find largest r with r*(r+1)/2 <= n */
static idx_t row_of(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* grow high until its triangular number exceeds n */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;
    /* binary search in [low, high] */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    if ((unsigned __int128)low * (low + 1) / 2 > n)
        low--;
    return low;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* locate the row r and column k for index n */
    idx_t r = row_of(n);
    idx_t t = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - t;

    /* allocate temporary rows of length r+1 */
    size_t len = (size_t)(r + 1);
    seq_t *prev = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(len, sizeof(seq_t));
    if (!prev || !curr) {
        /* allocation failed – return 0 as safe fallback */
        if (prev) free(prev);
        if (curr) free(curr);
        return 0;
    }

    /* row 0 */
    prev[0] = 1;

    /* build rows 1 … r */
    for (idx_t i = 1; i <= r; ++i) {
        /* left border (i+1)^3 */
        seq_t tmp = (seq_t)(i + 1);
        curr[0] = tmp * tmp * tmp;

        /* Pascal step */
        for (idx_t j = 1; j <= i; ++j) {
            seq_t left  = prev[j - 1];
            seq_t right = (j < i) ? prev[j] : (seq_t)0;
            curr[j] = left + right;
        }

        /* swap rows */
        seq_t *tmp_ptr = prev;
        prev = curr;
        curr = tmp_ptr;
    }

    seq_t result = prev[k];
    free(prev);
    free(curr);
    return result;
}
