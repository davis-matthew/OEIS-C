/*
 * A229283  Number of ascending runs in {1,...,9}^n.
 * https://oeis.org/A229283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(n) = 9^(n-1) * (5*n + 4) for n > 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute 9^(n-1) using binary exponentiation */
    seq_t result = 1;
    seq_t base   = 9;
    idx_t exp    = n - 1;
    while (exp > 0) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }

    /* Multiply by (5*n + 4) */
    seq_t mult = 5 * (seq_t)n + 4;
    return result * mult;
}
