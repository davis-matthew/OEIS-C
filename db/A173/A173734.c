/*
 * A173734  a(n) = (10^n + 62)/9 for n>0.
 * https://oeis.org/A173734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n + 62) / 9. Recurrence that the code uses: a(0) = 7,
 *            a(n) = 10 * a(n-1) - 62 for n ≥ 1. The sequence is defined for n
 *            ≥ 1; a(0) only appears in the recurrence
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* undefined for the offset; a(1) is the first listed term. */
    if (n == 0)
        return 0;

    seq_t result = (seq_t)7;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        result = result * (seq_t)10 - (seq_t)62;
    }
    return result;
}
