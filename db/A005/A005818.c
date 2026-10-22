/*
 * A005818  Numbers n that are primitive solutions to n^2 = a^2 + b^2 + c^2
 *          (a,b,c > 0).
 * https://oeis.org/A005818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Odd numbers excluding 1 and 5; a(1)=3, a(n)=2*n+3 for n>1
 * Range:     u8 n <= 126, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return 3;
    }
    return 2 * (seq_t)n + 3;
}
