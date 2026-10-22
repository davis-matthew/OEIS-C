/*
 * A003893  a(n) = Fibonacci(n) mod 10.
 * https://oeis.org/A003893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (a(n-1) + a(n-2)) mod 10, a(0) = 0, a(1) = 1; periodic
 *            with period 60
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t table[60] = {
        0, 1, 1, 2, 3, 5, 8, 3, 1, 4,
        5, 9, 4, 3, 7, 0, 7, 7, 4, 1,
        5, 6, 1, 7, 8, 5, 3, 8, 1, 9,
        0, 9, 9, 8, 7, 5, 2, 7, 9, 6,
        5, 1, 6, 7, 3, 0, 3, 3, 6, 9,
        5, 4, 9, 3, 2, 5, 7, 2, 9, 1
    };
    return table[n % 60];
}
