/*
 * A080586  A run of 3*2^n 1's followed by a run of 3*2^n 2's, for n=0, 1, 2,
 *          ...
 * https://oeis.org/A080586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for the first 3·2^k terms of each block k, a(n) = 2 for
 *            the next 3·2^k terms of each block k, where blocks are indexed
 *            by k = 0,1,2,…. The total length of block k is 6·2^k
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cumulative length processed so far */
    idx_t cum = 0;
    /* current power of two: 2^k (k grows from 0 upwards) */
    idx_t p2 = 1;

    while (1) {
        /* length of this entire block (ones + twos) = 6·2^k */
        idx_t blocklen = (idx_t)6 * p2;

        /* if the target index lies within this block */
        if (n < cum + blocklen) {
            idx_t offset = n - cum;                /* position inside block */
            /* first 3·2^k positions are 1, the rest 2 */
            if (offset < (idx_t)3 * p2)
                return (seq_t)1;
            else
                return (seq_t)2;
        }

        /* move to the next block */
        cum += blocklen;
        p2 <<= 1; /* next power of two */
    }
}
