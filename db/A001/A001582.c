/*
 * A001582  Product of Fibonacci and Pell numbers.
 * https://oeis.org/A001582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) + 2*a(n-3) - a(n-4), a(0) = 1, a(1) =
 *            2, a(2) = 10, a(3) = 36
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 10;
    if (n == 3) return 36;
    
    seq_t a0 = 1, a1 = 2, a2 = 10, a3 = 36;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t next = 2 * a3 + 7 * a2 + 2 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    
    return a3;
}
