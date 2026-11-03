/*
 * A185869  (Odd,even)-polka dot array in the natural number array A000027;
 *          read by antidiagonals.
 * https://oeis.org/A185869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) comes from the anti‑diagonal reading of the infinite array
 *            T(n,k) = 2n-1 + (n+k-1)*(2n+2k-3) (n,k >= 1). The (row, col)
 *            that supplies a(n) is found by locating the anti‑diagonal that
 *            contains the n‑th element. The number of elements up to diagonal
 *            s is (s-1)*s/2, so the desired diagonal s is...
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9877 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the anti‑diagonal index s such that the cumulative count up to
   s is >= n.  The function runs in O(log n) time. */
static idx_t anti_diagonal_of(idx_t n)
{
    idx_t lo = 1;
    idx_t hi = n + 1;              /* s cannot exceed n+1 */
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        /* Use 128‑bit to avoid overflow in mid*(mid-1)/2.          */
        if ((unsigned __int128)mid * (mid - 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Return the nth term of OEIS A185869 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* pragma: offset 1, n>=1 in use */

    /* Locate the anti‑diagonal containing the n‑th element. */
    idx_t s = anti_diagonal_of(n);

    /* Number of elements before this diagonal. */
    idx_t cum_before = (s - 2) * (s - 1) / 2;

    /* Position within the diagonal (1‑based). */
    idx_t pos   = n - cum_before;   /* 1 <= pos <= s-1 */

    /* Corresponding row and column in the array. */
    idx_t row = pos;
    idx_t col = s - pos;

    /* Compute T(row, col) in the required integer type. */
    seq_t n_r = (seq_t)row;       /* row   as seq_t */
    seq_t k_c = (seq_t)col;       /* column */

    seq_t term = 2 * n_r - 1;
    term += (n_r + k_c - 1) * (2 * n_r + 2 * k_c - 3);
    return term;
}
