/*
 * A145071  Partial sums of A000051, starting at n=1.
 * https://oeis.org/A145071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) + n - 2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^(n+1) by binary exponentiation (fast exponentiation). */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    idx_t  exp   = n + 1;          /* exponent is non‑negative */

    while (exp) {
        if (exp & 1)   result *= base;
        base *= base;
        exp >>= 1;
    }

    /* a(n) = result + n - 2 */
    result += (seq_t)n;
    result -= (seq_t)2;
    return result;
}
