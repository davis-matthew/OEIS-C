/*
 * A176710  Mix A001021, 2*A001021.
 * https://oeis.org/A176710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 12^(n/2) using exponentiation by squaring */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)12;
    idx_t  exp   = n / 2;   /* floor(n/2) */

    while (exp > 0) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    /* if n is odd multiply by 2 */
    if (n & 1U)
        result *= (seq_t)2;

    return result;
}
