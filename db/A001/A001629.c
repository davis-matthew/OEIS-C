/*
 * A001629  Self-convolution of Fibonacci numbers.
 * https://oeis.org/A001629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - a(n-4), n > 3; a(0)=0,
 *            a(1)=0, a(2)=1, a(3)=2
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 41, u64 n <= 86, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 2;
    
    seq_t a0 = 0, a1 = 0, a2 = 1, a3 = 2;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t a_new = (2 * a3 + a2) - (2 * a1 + a0);
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a_new;
    }
    
    return a3;
}
