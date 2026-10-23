/*
 * A145379  Square array read by antidiagonals upwards: T(n,k) = 1 if n
 *          divides k, T(n,k) = 0 otherwise, n >= 1, k >= 2.
 * https://oeis.org/A145379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145379: a(n) = 1 if the row that corresponds to the n-th entry of the
   array T(n,k)=1 iff n divides kอ่าน by antidiagonals upwards divides its column,
   otherwise 0. The mapping from the index n to the pair (row, column) follows:
   - Let s = row + column. The diagonals are processed for s = 3,4,5,...
   - Within each diagonal s, the elements are ordered from the bottom-left
     (largest row) to the top-right (row = 1). */

/* ai.c: implementation of a(idx_t n) */

seq_t a(idx_t n)
{
    /* Find minimal m such that m*(m+1)/2 >= n.  
       Here m = s - 2, the length of the current diagonal. */
    idx_t low  = 0;
    idx_t high = ((idx_t)1 << 32);   /* 2^32 is sufficient for all practical inputs */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        /* Use 128‑bit arithmetic to avoid overflow */
        if (( (__int128)mid * (mid + 1) ) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;                    /* length of the diagonal that contains n */

    /* Indexes before this diagonal: (m-1)*m/2 */
    idx_t prev = (idx_t)(( (__int128)(m - 1) * m ) / 2);

    idx_t offset = n - prev - 1;      /* 0‑based offset inside the diagonal */

    idx_t row = m - offset;           /* row index (n) */
    idx_t col = offset + 2;           /* column index (k) */

    /* Return 1 if row divides col, else 0 */
    return (seq_t)((col % row == 0) ? 1 : 0);
}
