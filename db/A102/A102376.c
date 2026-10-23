/*
 * A102376  a(n) = 4^A000120(n).
 * https://oeis.org/A102376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = 4^A000120(n)
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count the 1‑bits of n (number of 1’s in binary expansion). */
    unsigned int bits = 0;
    idx_t t = n;
    while (t) {
        bits += (t & 1);
        t >>= 1;
    }

    /* Compute 4^bits as seq_t using repeated multiplication. */
    seq_t result = 1;
    for (unsigned int i = 0; i < bits; ++i)
        result *= (seq_t)4;

    return result;
}
