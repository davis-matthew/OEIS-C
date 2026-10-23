/*
 * A137344  a(n)=4a(n-2). Also 3*A084221.
 * https://oeis.org/A137344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * a(n-2), with a(0) = 3, a(1) = 9
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence yields   a(2k)   = 3 * 4^k,
 *                  a(2k+1) = 9 * 4^k                         */
seq_t a(idx_t n)
{
    seq_t result;
    idx_t  exp = n >> 1;          /* exponent = floor(n/2)           */
    result = (n & 1) ? 9 : 3;     /* base for even/odd n             */

    while (exp-- > 0)             /* multiply by 4 exp times         */
        result *= 4;              /* or result <<= 2 for unsigned */

    return result;
}
