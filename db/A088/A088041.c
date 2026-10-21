/*
 * A088041  Smallest k such that k^4 - 1 is divisible by an n-th power, k > 1.
 * https://oeis.org/A088041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2, a(2) = a(3) = a(4) = 3, a(n) = 2^(n-2) - 1 for n >= 5
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2 || n == 3 || n == 4) return (seq_t)3;

    /* n >= 5 */
    seq_t result = (seq_t)1;   /* result = 2^(n-2) */
    seq_t base   = (seq_t)2;
    idx_t exp    = n - 2;      /* exponent is at most n-2 */

    /* fast exponentiation by squaring */
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }

    return result - (seq_t)1;
}
