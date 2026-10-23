/*
 * A181342  a(n) = (35*n^4 - 105*n^3 + 160*n^2 - 120*n + 36)/6.
 * https://oeis.org/A181342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (35*n^4 - 105*n^3 + 160*n^2 - 120*n + 36)/6
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 165, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for the intermediate calculation to avoid overflow
       on 64‑bit builds while staying compatible with 32‑bit and larger types. */
    unsigned __int128 nn = n;
    unsigned __int128 num = 35 * nn * nn * nn * nn
                          - 105 * nn * nn * nn
                          + 160 * nn * nn
                          - 120 * nn
                          + 36;
    return (seq_t)(num / 6);
}
