/*
 * A000244  Powers of 3: a(n) = 3^n.
 * https://oeis.org/A000244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, core
 * Formula:   a(n) = 3^n, computed by binary exponentiation
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base = 3;
    while (n > 0) {
        if (n & 1)
            result *= base;
        n >>= 1;
        if (n > 0)
            base *= base;
    }
    return result;
}
