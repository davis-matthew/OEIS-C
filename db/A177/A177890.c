/*
 * A177890  15-gonal (or pentadecagonal) pyramidal numbers: a(n) =
 *          n*(n+1)*(13*n-10)/6.
 * https://oeis.org/A177890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(13*n-10)/6
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a_val = (seq_t)n;                 /* n             */
    seq_t b_val = a_val + 1;                /* n + 1         */
    seq_t c_val = ((seq_t)13 * a_val) - 10; /* 13*n - 10     */

    /* eliminate factor 2 */
    if ((a_val & 1) == 0) {
        a_val >>= 1;
    } else if ((b_val & 1) == 0) {
        b_val >>= 1;
    } else {
        c_val >>= 1;
    }

    /* eliminate factor 3 */
    if (a_val % 3 == 0) {
        a_val /= 3;
    } else if (b_val % 3 == 0) {
        b_val /= 3;
    } else {
        c_val /= 3;      /* must be divisible */
    }

    return a_val * b_val * c_val;
}
