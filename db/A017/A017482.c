/*
 * A017482  a(n) = (11*n + 7)^10.
 * https://oeis.org/A017482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 7, u128 n <= 647,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (11*n + 7)^10
 */

seq_t a(idx_t n)
{
    /* compute the base 11*n + 7 using idx_t to avoid overflow at this stage */
    seq_t base = (seq_t)(11 * n + 7);
    seq_t res = 1;
    /* fast exponentiation for power 10 (fixed, small enough for a simple loop) */
    for (idx_t i = 0; i < 10; ++i)
        res *= base;
    return res;
}
