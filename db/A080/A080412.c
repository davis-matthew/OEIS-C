/*
 * A080412  Exchange rightmost two binary digits of n > 1; a(0)=0, a(1)=2.
 * https://oeis.org/A080412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) swaps the two least‑significant bits of n. For n < 2, this
 *            gives a(0)=0 and a(1)=2. The operation is its own inverse:
 *            a(a(n)) = n
 * Range:     u8 n <= 255, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the native sequence type */
    seq_t x = (seq_t)n;

    /* Swap bits 0 and 1 */
    seq_t res = (x & ~(seq_t)3)          /* keep all bits except the two LSBs   */
              | ((x & (seq_t)1) << 1)   /* move original LSB to bit 1          */
              | ((x >> 1) & (seq_t)1);  /* move original bit 1 to LSB          */

    return res;
}
