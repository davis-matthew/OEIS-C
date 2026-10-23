/*
 * A161382  (0,1)-sequence where n-th run has length n^2.
 * https://oeis.org/A161382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if the run number of n is odd, 1 if it is even. The run
 *            number is the unique integer m such that sum_{i=1}^{m-1} i^2 < n
 *            <= sum_{i=1}^{m} i^2. Each run has length m^2 and the parity of
 *            the run determines the value
 * Range:     u8 n <= 306, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure n >= 1 (values for n < 1 are undefined). */
    idx_t  i   = 1;          /* candidate run number */
    seq_t  sum = 0;          /* cumulative length of completed runs */
    seq_t  nseq = (seq_t)n;  /* cast once for comparison */

    /* Increase i until the n-th position lies within run i. */
    while (sum + (seq_t)i * (seq_t)i < nseq)
    {
        sum += (seq_t)i * (seq_t)i;
        i++;
    }

    /* Run number i is odd → value 0; even → value 1. */
    return (i & 1) ? 0 : 1;
}
