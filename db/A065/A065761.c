/*
 * A065761  Concatenation of increasing number of alternating digits in base
 *          2, starting with 0.
 * https://oeis.org/A065761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 0;  for n > 1
   a(n) = (a(n-1) << n) + ( n even ? ((1 << n) - 1) : 0 )   */
/* Computes OEIS sequence A065761 : concatenation of increasing
   alternating blocks of 0's and 1's in base‑2, starting with 0.  */

seq_t a(idx_t n)
{
    /* n is 1‑based; for n == 1 the value is 0. */
    seq_t val = 0;

    /* Build the blocks iteratively. */
    for (idx_t i = 2; i <= n; ++i) {
        /* Shift left by i bits: append a block of i zeros.   */
        val <<= (unsigned)i;           /* 'i' is at most 10000, fits in unsigned */

        /* If the block is of 1's (even index), add it.  */
        if ((i & 1u) == 0) {
            val += (((seq_t)1 << (unsigned)i) - 1);
        }
    }
    return val;
}
