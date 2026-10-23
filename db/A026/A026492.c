/*
 * A026492  a(n) = t(3n), where t = A001285 (Thue-Morse sequence).
 * https://oeis.org/A026492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + t(3*n), where t(k) is the Thue–Morse sequence 0 if
 *            popcount(k) is even, 1 if odd. Thus a(n) is 1 for even
 *            popcount(3*n) and 2 for odd popcount(3*n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 3*n in 128‑bit arithmetic to avoid overflow. */
    unsigned __int128 x = (unsigned __int128)n * 3;
    seq_t parity = 0;               /* will hold 0 or 1 */
    while (x) {                     /* toggle parity for each set bit */
        parity ^= 1;
        x &= x - 1;
    }
    return (seq_t)(1 + parity);     /* 1 or 2 */
}
