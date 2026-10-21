/*
 * A073267  Number of compositions (ordered partitions) of n into exactly two
 *          powers of 2.
 * https://oeis.org/A073267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (p, q) of powers of 2 with p + q
 *            = n
 * Range:     u8 n <= 8320, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* for n = 0 or 1 there are no positive powers of 2 summing to n */
    if (n < 2)
        return 0;

    seq_t count = 0;
    seq_t nseq  = (seq_t)n;
    seq_t p = 1;          /* current power of 2 */

    while (p <= nseq) {
        seq_t q = nseq - p;
        if (q > 0 && ((q & (q - 1)) == 0))
            count++;

        /* stop if the next power would overflow or exceed nseq */
        if (p > (nseq >> 1))
            break;
        p <<= 1;
    }

    return count;
}
