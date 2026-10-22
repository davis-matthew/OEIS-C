/*
 * A000749  a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3), n > 3, with
 *          a(0)=a(1)=a(2)=0, a(3)=1.
 * https://oeis.org/A000749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3), n > 3, with
 *            a(0)=a(1)=a(2)=0, a(3)=1
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return 0;
    if (n == 3) return 1;
    
    seq_t a_prev3 = 0;
    seq_t a_prev2 = 0;
    seq_t a_prev1 = 1;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t a_curr = 4 * a_prev1 - 6 * a_prev2 + 4 * a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    
    return a_prev1;
}
