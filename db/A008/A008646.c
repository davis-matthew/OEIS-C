/*
 * A008646  Molien series for cyclic group of order 5.
 * https://oeis.org/A008646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( (n+1)(n+2)(n+3)(n+4) / 120 )
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 289, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* which is equivalent to ( (n+1)(n+2)(n+3)(n+4) + 119 ) / 120 */

seq_t a(idx_t n)
{
    seq_t NN = (seq_t)n;            /* cast index to seq_t */
    seq_t prod = (NN + 1) * (NN + 2) * (NN + 3) * (NN + 4);
    prod += 119;                    /* add (denominator - 1) for ceil */
    return prod / 120;              /* integer division yields the result */
}
