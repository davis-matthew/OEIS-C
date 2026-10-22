/*
 * A002464  Hertzsprung's problem: ways to arrange n non-attacking kings on an
 *          n X n board, with 1 in each row and column. Also number of
 *          permutations of length n without rising or falling successions.
 * https://oeis.org/A002464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (n+1)*a(n-1) - (n-2)*a(n-2) - (n-5)*a(n-3) + (n-3)*a(n-4)
 *            Base cases: a(0)=1, a(1)=1, a(2)=0, a(3)=0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 1;
    if (n == 2 || n == 3) return 0;
    
    seq_t vals[4] = {1, 1, 0, 0};
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t coeff1 = (seq_t)(i + 1);
        seq_t coeff2 = (seq_t)(i - 2);
        seq_t coeff4 = (seq_t)(i - 3);
        
        seq_t ai = coeff1 * vals[3];
        ai -= coeff2 * vals[2];
        
        if (i >= 5) {
            seq_t coeff3 = (seq_t)(i - 5);
            ai -= coeff3 * vals[1];
        } else {
            ai += vals[1];
        }
        
        ai += coeff4 * vals[0];
        
        vals[0] = vals[1];
        vals[1] = vals[2];
        vals[2] = vals[3];
        vals[3] = ai;
    }
    
    return vals[3];
}
