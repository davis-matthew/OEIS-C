/*
 * A180598  Digital root of 8n.
 * https://oeis.org/A180598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = digital_root(8*n)
 * Range:     u8 n <= 31, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digital root: 1 + ((x - 1) % 9) for x > 0, and 0 for x == 0. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* 8*n fits easily into seq_t for any n in the tested range,
       but the multiplication is performed in seq_t to respect
       arbitrary‑precision builds. */
    seq_t m = (seq_t)8 * (seq_t)n;
    seq_t r = m % (seq_t)9;

    return (r == 0) ? (seq_t)9 : r;
}
