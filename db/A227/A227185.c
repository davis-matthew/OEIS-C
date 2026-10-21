/*
 * A227185  The largest part in the unordered partition encoded in the
 *          runlengths of the binary expansion of n.
 * https://oeis.org/A227185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 0 for n = 0; otherwise a(n) = (bit length of n) – (number
 *            of runs of identical bits in n) + 1
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t bits = 0;          /* number of bits in the binary representation of n */
    idx_t runs = 0;          /* number of runs of consecutive identical bits */
    idx_t prev_bit = (idx_t)-1; /* a value different from 0 and 1 */
    idx_t tmp = n;

    while (tmp)
    {
        bits++;                    /* count the bit */
        idx_t bit = tmp & 1;       /* current bit (0 or 1) */
        if (bit != prev_bit)       /* start of a new run */
            runs++;
        prev_bit = bit;
        tmp >>= 1;                 /* move to the next bit */
    }

    return (seq_t)(bits - runs + 1);
}
