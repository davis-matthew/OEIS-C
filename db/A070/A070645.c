/*
 * A070645  a(n) = n^6 mod 22.
 * https://oeis.org/A070645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 22
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 22 first to keep the numbers small. */
    idx_t mod = n % 22ULL;
    seq_t m = (seq_t)mod;
    /* Compute m^6 mod 22 iteratively. */
    seq_t r = 1;
    for (int i = 0; i < 6; ++i)
        r = (r * m) % 22;
    return r;
}
