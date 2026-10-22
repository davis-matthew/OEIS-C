/*
 * A007667  The sum of both two and three consecutive squares.
 * https://oeis.org/A007667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3), a(1) = 5, a(2) = 365,
 *            a(3) = 35645
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 5;
    if (n == 2) return 365;
    if (n == 3) return 35645;
    
    seq_t a1 = 5, a2 = 365, a3 = 35645;
    for (idx_t i = 4; i <= n; i++) {
        seq_t ai = (seq_t)99 * (a3 - a2) + a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
