/*
 * A131782  Triangle read by rows: (A004736 * A000012) + (A000012 * A004736) -
 *          A000012 as infinite lower triangular matrices.
 * https://oeis.org/A131782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131782:
   Triangle read by rows: (A004736 * A000012) + (A000012 * A004736) - A000012
   as infinite lower triangular matrices.
   The n-th row consists of the numbers
          A028387(r), A028387(r-1), … , A028387(1)
   where A028387(k) = k^2 + k - 1.
   The sequence is read row‑wise: a(1) = A028387(1),
   a(2) = A028387(2), a(3) = A028387(1), …
   For a given n (1‑based) let
      r = ceil((-1 + sqrt(8*n + 1))/2)   // row containing a(n)
      s = r*(r-1)/2                      // terms before that row
      j = n - s                          // position in the row (1‑based)
      k = r - j + 1                      // index in A028387
   then
      a(n) = k^2 + k - 1.
   All arithmetic that can overflow 64 bits is performed in
   unsigned __int128, which is supported on all compilers used for the
   test harness.
*/

static inline idx_t
row_of(idx_t n)                     /* smallest r with r(r+1)/2 >= n */
{
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (((unsigned __int128)mid * (mid + 1) / 2) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t
a(idx_t n)                          /* external linkage as required */
{
    if (n == 0)                     /* offset is 1; for safety */
        return 0;

    idx_t r      = row_of(n);                   /* row index */
    idx_t before = (unsigned __int128)r * (r - 1) / 2; /* terms before row r */
    idx_t j      = n - before;                  /* 1‑based column in row r */
    idx_t k      = r - j + 1;                   /* index in A028387 */

    seq_t kk = (seq_t)k;
    return kk * kk + kk - 1;                    /* A028387(k) */
}
