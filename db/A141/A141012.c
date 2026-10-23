/*
 * A141012  a(0) = 0, a(n) = 13^(n-1) + 1.
 * https://oeis.org/A141012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13^(n-1)+1 (a(0)=0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t exp   = n - 1;         /* exponent n-1 */
    seq_t result = (seq_t)1;     /* running product */
    seq_t base   = (seq_t)13;    /* base 13 */

    while (exp) {
        if (exp & 1ULL) result *= base;   /* multiply when bit set */
        base *= base;                     /* square base */
        exp >>= 1;                        /* shift exponent */
    }

    return result + (seq_t)1;              /* a(n) = 13^(n-1)+1 */
}
