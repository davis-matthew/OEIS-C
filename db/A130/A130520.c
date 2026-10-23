/*
 * A130520  a(n) = Sum_{k=0..n} floor(k/5). (Partial sums of A002266.)
 * https://oeis.org/A130520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)*(n-2)/10)
 * Range:     u8 n <= 52, u16 n <= 811, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the result is 0, avoid unsigned underflow */
    if (n < 3)
        return 0;

    /* Convert to seq_t once to avoid repeated casts */
    seq_t nseq = (seq_t)n;
    seq_t t = nseq - 1;
    seq_t s = nseq - 2;

    /* Compute (t * s) / 10.  Integer division performs the floor. */
    return (t * s) / (seq_t)10;
}
