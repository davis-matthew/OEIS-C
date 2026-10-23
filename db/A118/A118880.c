/*
 * A118880  a(n) is the cube of the sum of digits of n.
 * https://oeis.org/A118880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (A007953(n))^3
 * Range:     u8 n <= 6, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A007953(n) is the sum of decimal digits of n */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t x   = n;          /* work on a copy to preserve n if needed */
    while (x > 0) {
        sum += (seq_t)(x % 10);
        x   /= 10;
    }
    /* cube the digit sum */
    return sum * sum * sum;
}
