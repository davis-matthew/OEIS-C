/*
 * A199404  x-values in the solution to 13*x^2 - 12 = y^2.
 * https://oeis.org/A199404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = 11*a(n+2) - a(n) a(1) = 1, a(2) = 4, a(3) = 7, a(4) =
 *            43
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* initial four terms */
    static const seq_t init[4] = { 1, 4, 7, 43 };
    
    if (n <= 4) {
        return init[n - 1];
    }
    
    /* a_prev[0] = a_{i-1}, a_prev[1] = a_{i-2},
       a_prev[2] = a_{i-3}, a_prev[3] = a_{i-4} */
    seq_t a_prev[4] = { init[3], init[2], init[1], init[0] };
    seq_t cur = 0;
    
    for (idx_t i = 5; i <= n; ++i) {
        /* a_i = 11*a_{i-2} - a_{i-4} */
        cur = (seq_t)11 * a_prev[1] - a_prev[3];
        
        /* shift window for next iteration */
        a_prev[3] = a_prev[2];
        a_prev[2] = a_prev[1];
        a_prev[1] = a_prev[0];
        a_prev[0] = cur;
    }
    
    return cur;
}
