/*
 * A130404  Partial sums of A093178.
 * https://oeis.org/A130404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n odd) ? (n^2+3)/4 : (n^2+2n)/4
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset starts at 1 */
    if (n & 1) {                        /* n is odd */
        /* Use k = (n+1)/2  =>  a(n) = k*(k-1)+1  */
        seq_t k = ((seq_t)n + 1) >> 1;  /* (n+1)/2 */
        return k * (k - 1) + 1;
    } else {                            /* n is even */
        /* Use k = n/2  =>  a(n) = k*(k+1) */
        seq_t k = (seq_t)n >> 1;        /* n/2 */
        return k * (k + 1);
    }
}
