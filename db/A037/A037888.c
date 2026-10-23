/*
 * A037888  a(n) = (1/2)*Sum_{i} |d(i) - e(i)| where Sum_{i} d(i)*2^i is the
 *          base-2 representation of n and e(i) are digits d(i) in reverse
 *          order.
 * https://oeis.org/A037888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = Sum_{k=0}^{⌊len/2⌋-1} |A030308(n, k) - A030308(n,
 *            len-1-k)| * where len is the binary length of n. * This is the
 *            number of mismatched symmetric bit pairs in the binary *
 *            representation of n. The sequence is OEIS A037888
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS specification does not need n = 0, but guard against it. */
    if (n == 0)
        return 0;

    /* Determine the number of bits in n (len). */
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        len++;
        tmp >>= 1;
    }

    /* Count mismatched symmetric bit pairs. */
    idx_t mismatches = 0;
    /* Only examine the first half; the second half is handled by symmetry. */
    for (idx_t i = 0; i < len / 2; ++i) {
        idx_t bit_low  = (n >> i)         & 1;
        idx_t bit_high = (n >> (len - 1 - i)) & 1;
        if (bit_low != bit_high)
            ++mismatches;
    }

    return (seq_t)mismatches;
}
