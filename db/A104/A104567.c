/*
 * A104567  Triangle read by rows: T(i,j) = i-j+1 if j is odd; T(i,j) =
 *          2(i-j+1) if j is even (1 <= j <= i).
 * https://oeis.org/A104567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A104567
   a(n) = triangle read by rows:
   T(i,j) = i-j+1   if j is odd
            2*(i-j+1) if j is even
   (1 ≤ j ≤ i).  Row i has i elements.
   a(n) is the (i,j)-entry where i is the unique row satisfying
   i(i+1)/2 ≥ n > (i-1)i/2, and j = n - i(i-1)/2. */

/* Find the unique row i containing index n */
static inline idx_t row_of(idx_t n)
{
    idx_t i = 1;
    while (((unsigned __int128)i * (i + 1) / 2) < n)
        ++i;
    return i;
}

/* Compute a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* Input below offset is undefined */

    idx_t i = row_of(n);
    idx_t start = (unsigned __int128)i * (i - 1) / 2;   /* first index of row i */
    idx_t j = n - start;                                 /* column in the row */
    seq_t base = (seq_t)(i - j + 1);
    return (j & 1) ? base : base * 2;
}
