/*
 * A005522  a(n) = 1 + L(n) + F(2*n-1) with {L(n)}_{n>=0} the Lucas numbers
 *          (A000032) and F(2*n-1)_{n>=0} the bisected Fibonacci numbers
 *          (A001519).
 * https://oeis.org/A005522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + a(n-3) + 3*a(n-4) - a(n-5), with
 *            a(0) = 4, a(1) = 3, a(2) = 6, a(3) = 10, a(4) = 21
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 4;
    if (n == 1) return 3;
    if (n == 2) return 6;
    if (n == 3) return 10;
    if (n == 4) return 21;
    
    seq_t a0 = 4, a1 = 3, a2 = 6, a3 = 10, a4 = 21;
    
    for (idx_t i = 5; i <= n; i++) {
        seq_t next = 5 * a4 - 7 * a3 + a2 + 3 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    
    return a4;
}
