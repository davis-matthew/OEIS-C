/*
 * A097409  Initial decimal digit of n^5.
 * https://oeis.org/A097409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the most significant decimal digit of n⁵
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n⁵ using an intermediate 128‑bit type to avoid overflow
       even when seq_t is only 64 bits. */
    __int128 prod = (__int128)n;
    prod *= (__int128)n;
    prod *= (__int128)n;
    prod *= (__int128)n;
    prod *= (__int128)n;

    /* Extract the leading decimal digit. */
    while (prod >= 10)
        prod /= 10;

    return (seq_t)prod;
}
