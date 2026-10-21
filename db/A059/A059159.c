/*
 * A059159  A hierarchical sequence (W'2{2}*c) - see A059126.
 * https://oeis.org/A059159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4 * (largest power of 2 dividing n)
 * Range:     u8 n <= 63, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 1 using only integer arithmetic. */
seq_t a(idx_t n)
{
    /* n==0 is outside the OEIS offset; return 0. */
    if (n == 0) return 0;

    /* Count trailing zeros of n: the exponent of 2 in n. */
    int tz = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        ++tz;
    }

    /* 4 times 2^tz is the required value. */
    seq_t res = (seq_t)4;
    res <<= tz;          /* shift left tz positions = multiply by 2^tz */
    return res;
}
