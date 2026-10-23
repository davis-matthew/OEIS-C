/*
 * A040766  Continued fraction for sqrt(795).
 * https://oeis.org/A040766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction for sqrt(795) a(0) = 28 a(n) for n>0 repeats
 *            the pattern [5,9,5,56]
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)28;
    /* pattern repeats every 4 terms after the first term */
    static const seq_t pattern[4] = {5, 9, 5, 56};
    idx_t idx = (n - 1) % 4ULL;
    return pattern[idx];
}
