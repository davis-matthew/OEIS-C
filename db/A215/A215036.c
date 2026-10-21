/*
 * A215036  2 followed by "1,0" repeated.
 * https://oeis.org/A215036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A215036
   a(1) = 2
   a(n) = 1  if n is even   (n > 1)
   a(n) = 0  if n is odd    (n > 1)   */
/* Implements the simple periodic pattern 2,1,0,1,0,1,... */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2U;
    /* n>1 */
    /* Even n -> 1, odd n -> 0 */
    return (seq_t)((n & (idx_t)1) ? 0U : 1U);
}
