/*
 * A183041  Least number of knight's moves from (0,0) to (n,1) on infinite
 *          chessboard.
 * https://oeis.org/A183041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Piecewise formula for A183041:
   a(0) = 3;
   for n ≥ 1:
      if n % 4 == 0: a(n) = n/2 + 1
      if n % 4 == 1: a(n) = (n + 3)/2
      if n % 4 == 2: a(n) = n/2
      if n % 4 == 3: a(n) = (n + 1)/2
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    switch (n & 3)
    {
        case 0:  /* n % 4 == 0 */
            return ((seq_t)n) / 2 + 1;

        case 1:  /* n % 4 == 1 */
            return ((seq_t)n + 3) / 2;

        case 2:  /* n % 4 == 2 */
            return ((seq_t)n) / 2;

        case 3:  /* n % 4 == 3 */
            return ((seq_t)n + 1) / 2;

        default: /* unreachable */
            return 0;
    }
}
