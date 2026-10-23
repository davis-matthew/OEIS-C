/*
 * A172237  T(n,k) = T(n-1,k) + k*T(n-2,k) for k >= 1 and n >= 3 with T(0,k) =
 *          0 and T(1,k) = T(2,k) = 1 for all k >= 1; array T(n,k), read by
 *          descending antidiagonals, with n >= 0 and k >= 1.
 * https://oeis.org/A172237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A172237:  a(n) is obtained by reading the triangle T(n,k) along
   descending antidiagonals.  The triangle is defined by
      T(0,k) = 0,
      T(1,k) = T(2,k) = 1  for all k ≥ 1,
      T(n,k) = T(n-1,k) + k·T(n-2,k)  for n ≥ 3,   k ≥ 1.
   The global index n is mapped to a row n0 and column k0 by
   identifying the unique diagonal s with triangular(s-1) ≤ n < triangular(s),
   then setting  n0 = n - triangular(s-1)   (row, 0‑based)
                 k0 = s - n0               (column, 1‑based).  */

static inline idx_t triangular(idx_t s)
{
    return (idx_t)((( (__int128)s * (s + 1)) / 2));
}

seq_t a(idx_t n)
{
    /* locate the diagonal containing this index */
    idx_t s = 1;
    while (triangular(s) <= n)
        ++s;

    /* compute position within the diagonal */
    idx_t prevTri = triangular(s - 1);
    idx_t pos     = n - prevTri;     /* 0‑based offset on the diagonal */
    idx_t row     = pos;             /* row index (n0) */
    idx_t col     = s - pos;         /* column index k0 ≥ 1 */

    /* base cases directly from the definition */
    if (row == 0)
        return (seq_t)0;
    if (row == 1 || row == 2)
        return (seq_t)1;

    /* compute T(row,col) by the two‑term recurrence */
    seq_t prev2 = (seq_t)1;   /* T(1,col) */
    seq_t prev1 = (seq_t)1;   /* T(2,col) */
    seq_t curr  = (seq_t)1;   /* will hold current value */

    for (idx_t r = 3; r <= row; ++r) {
        curr = prev1 + (seq_t)col * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
