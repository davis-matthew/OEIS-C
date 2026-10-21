/*
 * A063143  Dimension of the space of weight 2n cusp forms for Gamma_0( 75 ).
 * https://oeis.org/A063143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 for n = 1 a(n) = 4 * (5 * n - 4) for n > 1 (derived
 *            from the conjectured closed‑form in the OEIS entry)
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset undefined, return 0 */
        return 0;

    if (n == 1)
        return (seq_t)5;

    /* Use unsigned 64‑bit arithmetic for the intermediate value
       so that larger seq_t sizes (up to 128 bits) can be handled
       by the cast to seq_t.  The result always fits in the
       tested range for this sequence. */
    uint64_t tmp = 4ULL * (5ULL * n - 4ULL);
    return (seq_t)tmp;
}
