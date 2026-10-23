/*
 * A026517  a(n) = t(5n), where t = A001285 (Thue-Morse sequence).
 * https://oeis.org/A026517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + popcount(5*n) mod 2 (Thue–Morse based)
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 5*n in full precision. */
    seq_t m = (seq_t)5 * n;

    /* Parity of the number of 1‑bits in m. */
    int parity = 0;
    seq_t t = m;
    while (t != 0) {
        parity ^= (int)(t & 1);
        t >>= 1;
    }

    /* Map 0→1 and 1→2. */
    return (seq_t)1 + (seq_t)parity;
}
