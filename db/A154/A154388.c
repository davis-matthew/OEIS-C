/*
 * A154388  Triangle T(n,k), 0<=k<=n, read by rows given by
 *          [0,1,-1,0,0,0,0,0,0,0,...] DELTA [1,-1,-1,1,0,0,0,0,0,0,0,...]
 *          where DELTA is the operator defined in A084938.
 * https://oeis.org/A154388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154388:  a(n) = 1 if n equals the last element of an odd‑indexed row
   or the second‑to‑last element of an even‑indexed row in the triangle T(n,k);
   otherwise a(n) = 0.
   The triangle rows are indexed by n (row 0 has one entry, row 1 has two, …). */

#include <stdint.h>
#include <math.h>

static idx_t row_from_index(idx_t n)
{
    /* Use a double approximation for the row, then correct by two
       successive comparisons to guarantee correctness. */
    idx_t r = (idx_t)((sqrtl((long double)8 * n + 1) - 1) / 2);

    /* Correct for over/underestimation due to floating point.  Use
       128‑bit arithmetic to avoid overflow during the comparisons. */
    while ((__int128)(r + 1) * (r + 2) / 2 <= n) ++r;
    while ((__int128)r * (r + 1) / 2 > n)  --r;

    return r;
}

seq_t a(idx_t n)
{
    /* Determine the row number r and column c within that row for index n. */
    idx_t r      = row_from_index(n);
    __int128 off = (__int128)r * (r + 1) / 2;
    idx_t c      = n - (idx_t)off;

    /* Base case: row 0 has a single 1. */
    if (r == 0) return (seq_t)1;

    /* For odd rows, the 1 is in the last position (c == r). */
    if ((r & 1) && c == r) return (seq_t)1;

    /* For even rows (r > 0), the 1 is in the second‑to‑last position (c == r-1). */
    if ((r & 1) == 0 && r > 0 && c == r - 1) return (seq_t)1;

    /* All other positions are 0. */
    return (seq_t)0;
}
