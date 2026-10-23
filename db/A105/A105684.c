/*
 * A105684  Weight distribution of [11,6,5]_3 ternary Golay perfect code.
 * https://oeis.org/A105684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Weight distribution of the [11,6,5]_3 ternary Golay perfect
 *            code. The weight enumerator is x^11 + 132*x^6*y^5 + 132*x^5*y^6
 *            + 330*x^3*y^8 + 110*x^2*y^9 + 24*y^11. Thus a(w) = number of
 *            codewords of weight w. For w > 11 the value is 0
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The code is linear over GF(3) of length 11.  Only weights
       0,5,6,8,9,11 occur; all others are zero. */
    if (n == 0)
        return (seq_t)1;
    if (n == 5 || n == 6)
        return (seq_t)132;
    if (n == 8)
        return (seq_t)330;
    if (n == 9)
        return (seq_t)110;
    if (n == 11)
        return (seq_t)24;
    /* For all other indices the weight enumerator is zero. */
    return (seq_t)0;
}
