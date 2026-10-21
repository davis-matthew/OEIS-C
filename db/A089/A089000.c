/*
 * A089000  Square table, read by antidiagonals, of coefficients T(k,n) (row
 *          k; column n) defined by: T(k,n) = k*T(k,n-1)+ n; T(k,0) = 0.
 * https://oeis.org/A089000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is obtained by reading the table of T(k, n) (row k, column
 *            n) by antidiagonals, where T(k, 0) = 0 and T(k, n) = k*T(k, n‑1)
 *            + n
 * Range:     u8 n <= 47, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine which antidiagonal d the index n falls on.
       The number of terms up to and including diagonal d is
       (d+1)(d+2)/2.  Find the smallest d with this value > n. */
    idx_t d = 0;
    for (;; d++) {
        /* Use 128‑bit intermediate to avoid overflow */
        unsigned __int128 count = (unsigned __int128)(d + 1) * (d + 2) / 2;
        if (count > n) break;
    }

    /* Offset of n within diagonal d */
    idx_t offset = n - (d * (d + 1)) / 2;  /* d*(d+1)/2 is terms before diag d */

    /* In each diagonal k increases from 0 to d.
       The column index is then n' = d - k.              */
    idx_t k = offset;
    idx_t ncol = d - offset;

    /* Compute T(k, ncol) by the recurrence. */
    seq_t val = 0;
    for (idx_t i = 1; i <= ncol; ++i) {
        val = val * (seq_t)k + (seq_t)i;
    }
    return val;
}
