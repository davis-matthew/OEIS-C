/*
 * A227417  Integer triples a(3n-2) = n, a(3n-1) = n+4, and a(3n) = n+7.
 * https://oeis.org/A227417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3n-2)=n, a(3n-1)=n+4, a(3n)=n+7
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is >=1 for a valid term */
    idx_t q = (n - 1) / 3;  /* block index (n-1)/3 */
    idx_t r = (n - 1) % 3;  /* position within block: 0,1,2 */

    seq_t block = (seq_t)q + 1;  /* n of the block */

    switch (r) {
        case 0:  return block;      /* a(3n-2) = n */
        case 1:  return block + 4;  /* a(3n-1) = n+4 */
        case 2:  return block + 7;  /* a(3n)   = n+7 */
        default: return block;      /* unreachable */
    }
}
