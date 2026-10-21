/*
 * A201455  a(n) = 3*a(n-1) + 4*a(n-2) for n>1, a(0)=2, a(1)=3.
 * https://oeis.org/A201455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n + (-1)^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0) return 2;   /* 4^0 + 1 = 2 */
    if (n == 1) return 3;   /* 4^1 - 1 = 3 */

    /* Compute 4^n using fast exponentiation */
    seq_t result = 1;   /* 4^0 */
    seq_t base   = 4;   /* base = 4 */

    idx_t exp = n;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }

    /* Add or subtract 1 depending on parity of n */
    if (n & 1) {        /* odd n: (-1)^n = -1 */
        result -= 1;
    } else {           /* even n: (-1)^n = +1 */
        result += 1;
    }
    return result;
}
