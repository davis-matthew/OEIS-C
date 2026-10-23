/*
 * A019300  First n elements of Thue-Morse sequence A010060 read as a binary
 *          number.
 * https://oeis.org/A019300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n‑1) + A010060(n), with a(0) = 0; A010060(n) is the
 *            n‑th Thue–Morse bit (parity of n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static unsigned int parity64(idx_t n)
{
    unsigned int p = 0;
    while (n) {
        p ^= 1;
        n &= n - 1;  /* remove lowest set bit */
    }
    return p;
}

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i)
        val = val * 2 + (seq_t)parity64(i);
    return val;
}
