/*
 * A161343  a(n) = 7^A000120(n).
 * https://oeis.org/A161343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^A000120(n) (A000120 = number of 1-bits in n)
 * Range:     u8 n <= 6, u16 n <= 62, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count the bits set in n (population count). */
    idx_t m = n;
    idx_t pop = 0;
    while (m != 0) {
        pop++;
        m &= m - 1;
    }

    /* Compute 7^pop as a seq_t. */
    seq_t result = 1;
    static const seq_t seven = 7;
    for (idx_t i = 0; i < pop; ++i)
        result *= seven;

    return result;
}
