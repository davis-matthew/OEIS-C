/*
 * A040482  Continued fraction for sqrt(505).
 * https://oeis.org/A040482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 22; for n > 0, a(n) repeats the pattern {2, 8, 2, 44} */
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)22;

    static const seq_t pattern[4] = { 2, 8, 2, 44 };
    return pattern[(n - 1) & 3];
}
