/*
 * A132226  Placement sequence for the dense normalized fractal sequence
 *          A132224.
 * https://oeis.org/A132226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 For n > 1, let k = ⌊log₂(n‑1)⌋. Then a(n) = 2 * (2ᵏ +
 *            1) – n (i.e. a(n) = 2*start – n where start = 2ᵏ + 1)
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset */
    if (n <= 1)
        return (seq_t)1;

    /* Compute k = floor(log₂(n‑1)) by counting the highest set bit */
    idx_t m = n - 1;
    idx_t k = 0;
    idx_t t = m;
    while (t > 1) {
        t >>= 1;
        ++k;
    }

    /* start = 2ᵏ + 1 */
    idx_t start = (1ULL << k) + 1;

    /* a(n) = 2*start – n */
    seq_t res = (seq_t)start;
    res <<= 1;          /* 2*start */
    res -= (seq_t)n;    /* subtract n */

    return res;
}
