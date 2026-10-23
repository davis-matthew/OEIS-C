/*
 * A036294  a(n) = n*8^n.
 * https://oeis.org/A036294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*8^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 8^n by binary exponentiation. */
    seq_t pow8 = (seq_t)1;
    seq_t base = (seq_t)8;
    idx_t exp = n;

    while (exp > 0)
    {
        if (exp & 1u)
            pow8 = pow8 * base;
        base = base * base;
        exp >>= 1;
    }
    return (seq_t)n * pow8;
}
