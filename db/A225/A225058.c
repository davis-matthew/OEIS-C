/*
 * A225058  a(4*n) = n-1. a(2*n+1) = a(4*n+2) = 2*n+1.
 * https://oeis.org/A225058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(4*n) = n-1; a(2*n+1) = a(4*n+2) = 2*n+1
 * Range:     i8 n <= 128, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n mod 4 distinguishes the three cases
       0 → a(n) = n/4 – 1
       1 → a(n) = n          (odd indices)
       2 → a(n) = n/2
       3 → a(n) = n          (odd indices) */
    idx_t mod = n & 3;
    switch (mod) {
        case 0:
            return (seq_t)(n >> 2) - 1;   /* a(4*k) = k-1 */
        case 2:
            return (seq_t)(n >> 1);        /* a(4*k+2) = (4*k+2)/2 = 2*k+1 */
        default:                         /* cases 1 and 3 : odd indices */
            return (seq_t)n;
    }
}
