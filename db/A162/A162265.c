/*
 * A162265  a(n) = (2*n^3 + 5*n^2 - 5*n)/2.
 * https://oeis.org/A162265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 5*n^2 - 5*n)/2
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Return a(1) = 1, a(2) = 13, etc., for n >= 1. */
    if (n == 0) return 0;  /* not used except to keep the function defined for n = 0 */
    seq_t sn = (seq_t)n;
    seq_t res = sn * sn;          /* n^2 */
    res *= sn;                    /* n^3 */
    res *= 2;                     /* 2*n^3 */
    seq_t term2 = sn * sn;         /* n^2 */
    term2 *= 5;                   /* 5*n^2 */
    seq_t term3 = sn * 5;          /* 5*n */
    res += term2;                 /* add 5*n^2 */
    res -= term3;                 /* subtract 5*n */
    res /= 2;                     /* divide by 2 */
    return res;
}
