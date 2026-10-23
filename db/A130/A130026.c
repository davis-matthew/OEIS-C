/*
 * A130026  Triangle (n,k) by columns, arithmetic sequences interspersed with
 *          k zeros.
 * https://oeis.org/A130026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A130026: Triangle (n,k) by columns, arithmetic sequences interspersed
   with k zeros.  A linear index n is first decomposed into its row
   r and column c (offset_row = r(r+1)/2).  Then
   a(n) = r - c + 1 if (r - c) % (c + 1) == 0
          0           otherwise. */
#include <stdint.h>
#include <stdbool.h>

/* Compute triangular number r*(r+1)/2 using 128‑bit arithmetic. */
static inline __int128 triangular_idx(idx_t r)
{
    return (__int128)r * (r + 1) / 2;
}

/* Find the largest r with r*(r+1)/2 <= n.  Uses a doubling proof
   of upper bound followed by binary search.  No floating‑point or
   sqrt is required, making it valid when seq_t is an arbitrary‑
   precision integer class. */
static inline idx_t _row_of_n(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = 1;

    /* Grow hi until triangular(hi) > n. */
    while (triangular_idx(hi) <= (__int128)n) {
        lo = hi;
        if (hi > UINT64_MAX / 2) {
            hi = UINT64_MAX;
            break;
        }
        hi <<= 1;  /* double hi */
    }

    /* Binary search between lo (≤ r) and hi (> r). */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (triangular_idx(mid) <= (__int128)n) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t r = _row_of_n(n);
    /* starting index of row r */
    __int128 start128 = triangular_idx(r);
    idx_t start = (idx_t)start128;   /* safe; start <= n */

    idx_t c = n - start;             /* column within the row */

    idx_t diff   = r - c;            /* r - c */
    idx_t denom  = c + 1;            /* c + 1 */

    if (denom == 0) return 0;        /* should never happen */
    if (diff % denom == 0) {
        return (seq_t)(diff + 1);    /* value is r - c + 1 */
    } else {
        return 0;                    /* zero inserted */
    }
}
