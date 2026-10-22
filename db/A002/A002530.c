/*
 * A002530  a(n) = 4*a(n-2) - a(n-4) for n > 1, a(n) = n for n = 0, 1.
 * https://oeis.org/A002530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac, core, nice
 * Formula:   a(n) = 4*a(n-2) - a(n-4) for n >= 4, a(0)=0, a(1)=1, a(2)=1,
 *            a(3)=3
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 68, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 3;
    
    seq_t prev4 = 0;
    seq_t prev3 = 1;
    seq_t prev2 = 1;
    seq_t prev1 = 3;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t curr = 4 * prev2 - prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
