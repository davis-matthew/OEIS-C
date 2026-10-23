/*
 * A179058  Number of non-attacking placements of 3 rooks on an n X n board.
 * https://oeis.org/A179058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n*(n-1)*(n-2))^2) / 6 (for n >= 3, a(1)=a(2)=0)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 55, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return 0;                 /* a(1) = 0, a(2) = 0 */
    seq_t nn = (seq_t)n;
    /* compute C = n*(n-1)*(n-2)/6 first to keep intermediates small */
    seq_t t = (nn * (nn - 1) * (nn - 2)) / 6;
    return t * t * 6;                     /* 6 * C^2 = a(n) */
}
