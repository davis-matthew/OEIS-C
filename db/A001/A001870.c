/*
 * A001870  Expansion of (1-x)/(1 - 3*x + x^2)^2.
 * https://oeis.org/A001870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4), a(0)=1, a(1)=5,
 *            a(2)=19, a(3)=65
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 19;
    if (n == 3) return 65;
    
    seq_t a0 = 1, a1 = 5, a2 = 19, a3 = 65;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t temp = 6 * a3 - 11 * a2 + 6 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = temp;
    }
    
    return a3;
}
