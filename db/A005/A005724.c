/*
 * A005724  Coefficient of x^(3*n-1) and of x^(3*n+1) in the expansion of
 *          (1+x+x^2+x^3)^(2*n).
 * https://oeis.org/A005724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^(3*n-1) in (1+x+x^2+x^3)^(2*n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t two_n = 2 * n;
    idx_t three_n_minus_1 = 3 * n - 1;
    
    /* Expand (1+x+x^2+x^3)^(2*n) and find coefficient of x^(3*n-1).
     * Using multinomial expansion with variables a, b, c, d representing
     * counts of 1, x, x^2, x^3 factors respectively.
     * Constraint: a+b+c+d = 2*n and power b+2*c+3*d = 3*n-1.
     * Coefficient is (2*n)! / (a!*b!*c!*d!).
     */
    
    for (idx_t d = 0; d * 3 <= three_n_minus_1; d++) {
        for (idx_t c = 0; 2 * c + 3 * d <= three_n_minus_1; c++) {
            idx_t b = three_n_minus_1 - 2 * c - 3 * d;
            
            /* Check a = two_n - b - c - d >= 0, equivalent to c + 2*d >= n - 1 */
            if (c + 2 * d >= n - 1) {
                idx_t a_val = two_n - b - c - d;
                
                /* Multinomial coefficient computed as chained binomials:
                 * (2*n)! / (a!*b!*c!*d!) = C(2*n,a) * C(2*n-a,b) * C(2*n-a-b,c)
                 */
                seq_t coeff = binom(two_n, a_val);
                coeff *= binom(two_n - a_val, b);
                coeff *= binom(two_n - a_val - b, c);
                result += coeff;
            }
        }
    }
    
    return result;
}
