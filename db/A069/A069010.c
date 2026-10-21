/*
 * A069010  Number of runs of 1's in the binary representation of n.
 * https://oeis.org/A069010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of runs of 1's in the binary representation of n
 * Range:     u8 n <= 8180, u16 n <= 8180, u32 n <= 8180, u64 n <= 8180,
 *            u128 n <= 8180, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n;
    seq_t runs = 0;
    int prev = 0;          /* previous bit (0 or 1) */
    while (x) {
        int bit = x & 1;   /* current least significant bit */
        if (bit && !prev)  /* a new run starts when a 1 follows a 0 */
            runs++;
        prev = bit;        /* update previous bit */
        x >>= 1;           /* shift to process next bit */
    }
    return (seq_t)runs;
}
