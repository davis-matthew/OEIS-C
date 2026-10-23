/*
 * A187028  Number of 3-step one or two collinear space at a time queen's
 *          tours on an n X n board summed over all starting positions.
 * https://oeis.org/A187028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 240*n^2 - 936*n + 912 for n>3; a(1) = 0, a(2) = 24, a(3)
 *            = 296
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 3) {
        switch ((idx_t)n) {
            case 1: return (seq_t)0;
            case 2: return (seq_t)24;
            case 3: return (seq_t)296;
            default: return (seq_t)0; /* for n == 0, though not used */
        }
    }
    seq_t nn = (seq_t)n;
    return (seq_t)240 * nn * nn - (seq_t)936 * nn + (seq_t)912;
}
