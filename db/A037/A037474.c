/*
 * A037474  a(n) = Sum{d(i)*8^i: i=0,1,...,m}, where Sum{d(i)*7^i:
 *          i=0,1,...,m} is the base 7 representation of n.
 * https://oeis.org/A037474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum{d(i) * 8^i : i = 0,1,...,m} where d(i) are the digits
 *            of n in base 7
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;          /* 8^i */
    idx_t x = n;
    while (x > 0) {
        /* digit in base 7 (0..6) */
        seq_t d = (seq_t)(x % 7);
        result += d * power;
        power *= 8u;
        x /= 7;
    }
    return result;
}
