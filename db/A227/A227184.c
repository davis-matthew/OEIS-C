/*
 * A227184  a(n) = product of parts of the unordered partition encoded with
 *          the runlengths of binary expansion of n.
 * https://oeis.org/A227184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = product of the cumulative sums of (run length minus 1 for
 *            all runs except the first) of the binary expansion of n, read
 *            from LSB to MSB
 * Range:     u8 n <= 79, u16 n <= 1359, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t product = 1;
    seq_t cum     = 0;

    idx_t temp = n;
    int first  = 1;

    while (temp)
    {
        idx_t bit    = temp & 1;
        idx_t runlen = 0;
        while (temp && ((temp & 1) == bit))
        {
            ++runlen;
            temp >>= 1;
        }

        seq_t adj = first ? (seq_t)runlen : (seq_t)(runlen - 1);
        cum += adj;
        product *= cum;
        first = 0;
    }

    return product;
}
