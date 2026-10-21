/*
 * A062813  a(n) = Sum_{i=0..n-1} i*n^i.
 * https://oeis.org/A062813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n-1} i * n^i
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow = 1;          /* n^0 */
    idx_t i;

    for (i = 0; i < n; ++i) {
        if (i > 0)
            pow *= (seq_t)n;          /* update to n^i */

        seq_t term = ((seq_t)i) * pow; /* i * n^i */
        result += term;
    }

    return result;
}
