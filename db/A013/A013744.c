/*
 * A013744  a(n) = 9^(3*n + 1).
 * https://oeis.org/A013744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^(3*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute a(n) = 9 * 729^n  by binary exponentiation (O(log n)) */
    seq_t result = 1;
    seq_t base   = (seq_t)729;   /* 3^6 = 729 */
    idx_t exp = n;               /* exponent n */
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result * (seq_t)9;
}
