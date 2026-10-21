/*
 * A070646  a(n) = n^6 mod 23.
 * https://oeis.org/A070646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 23
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 23 first; 23 < 2^8 so 64‑bit arithmetic suffices. */
    uint64_t nm = (uint64_t)n % 23ULL;

    /* Compute nm^6 mod 23 with a small loop. */
    uint64_t r = 1;
    for (int i = 0; i < 6; ++i)
        r = (r * nm) % 23ULL;

    return (seq_t)r;
}
