/*
 * A129565  A115359 * A000012 as infinite lower triangular matrices.
 * https://oeis.org/A129565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry in the infinite lower triangular matrix
 *            where odd rows consist entirely of 1's and even rows consist of
 *            n/2 zeros followed by n/2 ones
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r for which the cumulative count of entries reaches or
       exceeds n.  cum holds the number of entries up to row r. */
    idx_t r   = 1;
    idx_t cum = 1;              // 1*(1+1)/2
    while (n > cum) {
        ++r;
        cum += r;               // add number of entries in new row r
    }
    idx_t prev_cum = cum - r;   // entries up to previous row
    idx_t k = n - prev_cum;     // column index within row r (1‑based)

    /* Determine the entry value. */
    if (r & 1) {                // odd row → all ones
        return (seq_t)1;
    } else {                    // even row
        return (k <= r / 2) ? (seq_t)0 : (seq_t)1;
    }
}
