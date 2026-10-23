/*
 * A024378  a(n) = 2nd elementary symmetric function of the first n+1 positive
 *          integers congruent to 1 mod 4.
 * https://oeis.org/A024378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(12*n^2+8*n-5)/6
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 213, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Guard against n == 0, though the sequence is defined only for n >= 1. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Convert the index to the arbitrary‐precision type first. */
    seq_t nt = (seq_t)n;

    /* Compute the quadratic part: 12*n^2 + 8*n - 5 */
    seq_t quad = (seq_t)12 * nt * nt + (seq_t)8 * nt - (seq_t)5;

    /* Compute the product n*(n+1) */
    seq_t prod = nt * (nt + (seq_t)1);

    /* Final value:  n*(n+1)*(12*n^2+8*n-5) / 6  */
    seq_t result = prod * quad / (seq_t)6;

    return result;
}
