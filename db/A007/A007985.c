/*
 * A007985  Co-growth function of the group < a, b, a^{-1}, b^{-1} | b >,
 *          isomorphic to Z (counting only reduced words).
 * https://oeis.org/A007985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: n*a(n) + (-3*n+1)*a(n-1) + 2*(n+1)*a(n-2) - 6*n*a(n-3) + 3*(-n+6)*a(n-4) + 9*(n-5)*a(n-5) = 0; a(1)=2, a(2)=2, a(3)=6, a(4)=22, a(5)=58 */

seq_t a(idx_t n) {
    if (n == 1) return 2;
    if (n == 2) return 2;
    if (n == 3) return 6;
    if (n == 4) return 22;
    if (n == 5) return 58;
    
    seq_t a1 = 2, a2 = 2, a3 = 6, a4 = 22, a5 = 58;
    
    for (idx_t i = 6; i <= n; i++) {
        seq_t ni = (seq_t)i;
        seq_t numerator = (3*ni - 1) * a5 
                        - 2 * (ni + 1) * a4 
                        + 6 * ni * a3 
                        + 3 * (ni - 6) * a2 
                        - 9 * (ni - 5) * a1;
        seq_t ai = numerator / ni;
        
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = ai;
    }
    
    return a5;
}
