/*
 * A109595  n^3 followed by n^2 followed by n.
 * https://oeis.org/A109595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2)/3)^((-3*(n mod 3)^2+7*(n mod 3)+2)/2)
 * Range:     u8 n <= 18, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute the base m = floor((n+2)/3) */
    seq_t m = ((seq_t)n + 2) / 3;
    /* Determine exponent based on n mod 3 */
    idx_t r = n % 3;
    switch (r) {
        case 0: /* exponent = 1 */
            return m;
        case 1: /* exponent = 3 */
            return m * m * m;
        case 2: /* exponent = 2 */
            return m * m;
        default: /* unreachable, but keep compiler happy */
            return 0;
    }
}
