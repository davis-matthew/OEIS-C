/*
 * A086799  Replace all trailing 0's with 1's in binary representation of n.
 * https://oeis.org/A086799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + 2^v2(n) – 1, where v2(n) is the number of trailing *
 *            zeros in the binary representation of n (the exponent of 2
 *            dividing n). * For odd n this reduces to a(n) = n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count trailing zeros of n. */
    idx_t tmp = n;
    unsigned int t = 0;
    while ((tmp & (idx_t)1) == 0) {
        tmp >>= 1;
        ++t;
    }

    /* Compute 2^t safely in the type seq_t. */
    seq_t shift = ((seq_t)1 << t);

    /* a(n) = n + 2^t – 1 */
    return (seq_t)n + shift - (seq_t)1;
}
