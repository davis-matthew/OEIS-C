/*
 * A033128  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,0,1.
 * https://oeis.org/A033128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10*a(n-1) + {1,0,1}[(n-1) mod 3], a(1) = 1, which
 *            generates the integer whose decimal digits are the first n terms
 *            of the periodic sequence 1,0,1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If the index is 0 (outside the OEIS offset) return 0. */
    if (n == 0) return (seq_t)0;

    /* Digits repeating 1,0,1. */
    static const seq_t pattern[3] = { (seq_t)1, (seq_t)0, (seq_t)1 };

    /* Start with the first digit (n = 1). */
    seq_t result = pattern[0];

    /* Append the remaining digits one by one. */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* Multiply current number by 10 (shift decimal left)
           and add the next digit according to the cycle. */
        result = result * (seq_t)10 + pattern[(i - 1) % 3];
    }

    return result;
}
