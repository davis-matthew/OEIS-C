/*
 * A001794  Negated coefficients of Chebyshev T polynomials: [x^n](-T(n+6,
 *          x)), n >= 0.
 * https://oeis.org/A001794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2)*(n+1)*(n+2)*(n+6)/3
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 21, u64 n <= 50, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t p1 = nn + 1;
    seq_t p2 = nn + 2;
    seq_t p6 = nn + 6;
    
    // Exactly one of p1, p2, p6 is divisible by 3; divide it first to avoid overflow
    if (p1 % 3 == 0) {
        p1 = p1 / 3;
    } else if (p2 % 3 == 0) {
        p2 = p2 / 3;
    } else {
        p6 = p6 / 3;
    }
    
    seq_t product = p1 * p2 * p6;
    
    // Apply the 2^(n-2) factor
    if (n >= 2) {
        product = product << (n - 2);
    } else if (n == 1) {
        product = product / 2;
    } else {  // n == 0
        product = product / 4;
    }
    
    return product;
}
