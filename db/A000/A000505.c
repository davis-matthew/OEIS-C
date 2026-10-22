/*
 * A000505  Eulerian numbers (Euler's triangle: column k=5 of A008292, column
 *          k=4 of A173018).
 * https://oeis.org/A000505
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - (n+1)*4^n + C(n+1,2)*3^n - C(n+1,3)*2^n + C(n+1,4)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"


seq_t a(idx_t n) {
    seq_t n_s = (seq_t)(n + 1);
    
    // Compute powers
    seq_t pow5 = a000272_ipow((seq_t)5, n);
    seq_t pow4 = a000272_ipow((seq_t)4, n);
    seq_t pow3 = a000272_ipow((seq_t)3, n);
    seq_t pow2 = a000272_ipow((seq_t)2, n);
    
    // Binomial coefficients
    seq_t c2 = n_s * (n_s - 1) / 2;
    seq_t c3 = n_s * (n_s - 1) * (n_s - 2) / 6;
    seq_t c4 = n_s * (n_s - 1) * (n_s - 2) * (n_s - 3) / 24;
    
    // Combine terms
    seq_t result = pow5 - n_s * pow4 + c2 * pow3 - c3 * pow2 + c4;
    return result;
}
