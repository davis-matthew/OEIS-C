/*
 * A059137  A hierarchical sequence (W3{2,2}cc - see A059126).
 * https://oeis.org/A059137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 18 * (1 + 2 * t(n)) where t(n) is the number of
 *            consecutive 1‑bits at the LSB of n. This follows from the
 *            recurrence A037227(n) = 1 for even n, and A037227(n) =
 *            A037227((n-1)/2) + 2 for odd n
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* count trailing 1-bits */
    idx_t cnt = 0;
    idx_t m = n;
    while ((m & 1) != 0) {
        ++cnt;
        m >>= 1;
    }

    /* A037227(n) = 1 + 2*cnt */
    seq_t val = (seq_t)1 + (seq_t)2 * (seq_t)cnt;

    /* a(n) = 18 * A037227(n) */
    return val * (seq_t)18;
}
