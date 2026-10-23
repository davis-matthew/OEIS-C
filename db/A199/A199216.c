/*
 * A199216  a(n) = 6*5^n+1.
 * https://oeis.org/A199216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*5^n+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 5;
    idx_t exp = n;

    /* fast exponentiation: base = 5, result = 5^exp */
    while (exp) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    result = result * (seq_t)6 + 1;
    return result;
}
