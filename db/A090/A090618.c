/*
 * A090618  Exponent of highest power of 9 dividing n!.
 * https://oeis.org/A090618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (floor(n/3) + floor(n/9) + floor(n/27) + ...) / 2
 *            )
 * Range:     u8 n <= 1027, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t div = n;
    while (div >= 3) {
        div /= 3;
        sum += (seq_t)div;
    }
    return sum / 2;
}
