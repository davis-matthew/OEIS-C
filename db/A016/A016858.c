/*
 * A016858  a(n) = (5n)^10.
 * https://oeis.org/A016858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5n)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 16, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 5^10 */
    const seq_t factor = (seq_t)9765625;

    seq_t pown = (seq_t)1;
    for (idx_t i = 0; i < 10; ++i)
        pown *= (seq_t)n;

    return pown * factor;
}
