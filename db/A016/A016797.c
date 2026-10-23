/*
 * A016797  a(n) = (3*n + 2)^9.
 * https://oeis.org/A016797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n + 2)^9
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base value 3*n + 2 */
    seq_t base = (seq_t)n;
    base = base * 3 + 2;

    /* Raise the base to the 9th power by successive multiplication. */
    seq_t result = 1;
    for (idx_t i = 0; i < 9; ++i) {
        result *= base;
    }

    return result;
}
