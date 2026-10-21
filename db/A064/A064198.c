/*
 * A064198  a(n) = 3*(n-2)*(n-3)*(3*n^2-3*n-8)/2.
 * https://oeis.org/A064198
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n-2)*(n-3)*(3*n^2-3*n-8)/2
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 176, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Terms for n=2 and n=3 are zero; avoid unsigned underflow. */
    if (n < 4) return 0;

    seq_t tn = (seq_t)n;
    seq_t term1 = tn - 2;                 /* (n-2) */
    seq_t term2 = tn - 3;                 /* (n-3) */
    seq_t term3 = 3 * tn * tn - 3 * tn - 8; /* (3*n^2-3*n-8) */

    /* Compute the product and divide by 2. All intermediate values fit
       within the natural width of seq_t for the intended range of n. */
    return 3 * term1 * term2 * term3 / 2;
}
