/*
 * A038502  Remove 3's from n.
 * https://oeis.org/A038502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n / 3^{v_3(n)} = largest divisor of n not divisible by 3
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    while (x % (seq_t)3 == 0)
        x /= (seq_t)3;
    return x;
}
