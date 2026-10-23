/*
 * A016932  a(n) = (6*n + 1)^12.
 * https://oeis.org/A016932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 6, u128 n <= 270,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (6*n + 1)^12
 *
 * Computes the n-th term of OEIS sequence A016932.
 * The sequence is defined directly, no memoisation needed.
 */
seq_t a(idx_t n)
{
    /* base = 6*n + 1  (conversion to seq_t is guaranteed) */
    seq_t base = (seq_t)n * 6 + 1;

    /* compute base^12 by repeated multiplication */
    seq_t result = 1;
    for (idx_t i = 0; i < 12; ++i)
        result = result * base;

    return result;
}
