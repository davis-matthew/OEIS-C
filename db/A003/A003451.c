/*
 * A003451  Number of nonequivalent dissections of an n-gon into 3 polygons by
 *          nonintersecting diagonals up to rotation.
 * https://oeis.org/A003451
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-4)*(2*n^2-4*n-3*(1-(-1)^n))/24
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    
    if (n % 2 == 0) {
        // Even: (n-4)*n*(n-2)/12
        return (nn - 4) * nn * (nn - 2) / 12;
    } else {
        // Odd: (n-4)*(n-3)*(n+1)/12
        return (nn - 4) * (nn - 3) * (nn + 1) / 12;
    }
}
