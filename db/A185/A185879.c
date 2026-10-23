/*
 * A185879  Weight array of A185877, by antidiagonals.
 * https://oeis.org/A185879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry of the weight array for A185877 read by
 *            antidiagonals. The weight array T(i,j) is defined by: T(1,1) =
 *            1, T(1,k) = 2*(k-1) for k>1, T(i,1) = 0 for i>1, T(i,k) = 2 for
 *            i>1 and k>1. a(n) is T(i,j), where (i,j) is the cell visited
 *            when enumerating the array by antidiagonals startin...
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: triangular number k*(k+1)/2, computed safely in 128-bit. */
static idx_t triangular(idx_t k)
{
    return (idx_t)((unsigned __int128)k * (k + 1) / 2);
}

/* Helper: find the diagonal index d such that n <= d*(d+1)/2.
   Uses exponential search followed by binary search, with 128‑bit arithmetic. */
static idx_t diag_index(idx_t n)
{
    idx_t lo = 1, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 < n)
        hi <<= 1;                       /* double aggressively */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Determine diagonal d and position offset within it. */
    idx_t d = diag_index(n);                     /* d >= 1 */
    idx_t prev_tri = triangular(d - 1);          /* T(d-1) */
    idx_t offset = n - prev_tri;                 /* 1 .. d */
    idx_t row = offset;                          /* 1 .. d */
    idx_t col = d + 1 - offset;                  /* 1 .. d */

    /* Compute the weight element according to the definition. */
    if (row == 1) {
        if (col == 1)
            return (seq_t)1;                     /* special case T(1,1) */
        else
            return (seq_t)2 * (seq_t)(col - 1);  /* T(1,k), k>1 */
    } else if (col == 1) {
        return (seq_t)0;                         /* T(i,1), i>1 */
    } else {
        return (seq_t)2;                         /* T(i,k), i>1 and k>1 */
    }
}
