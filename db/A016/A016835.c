/*
 * A016835  a(n) = (4*n + 2)^11.
 * https://oeis.org/A016835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 2)^11
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 13, u128 n <= 795,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to seq_t before arithmetic to avoid truncation */
    seq_t base = (seq_t)n * 4 + 2;
    seq_t result = 1;
    /* 11 is a small constant; a straightforward loop is fast and exact */
    for (int i = 0; i < 11; ++i)
        result *= base;
    return result;
}
