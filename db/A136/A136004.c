/*
 * A136004  a(n) = A005811(n) + 4.
 * https://oeis.org/A136004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005811(n) + 4
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A005811(n) = number of runs of equal bits in the binary
   representation of n (with A005811(0)=0) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 4;   /* A005811(0) = 0 */

    seq_t runs = 1;                /* at least one run in n > 0 */
    idx_t prev = n & 1;            /* least significant bit */
    n >>= 1;                       /* shift right to process remaining bits */

    while (n) {
        idx_t bit = n & 1;
        if (bit != prev)
            runs++;
        prev = bit;
        n >>= 1;
    }
    return runs + 4;                /* A005811(n) + 4 */
}
