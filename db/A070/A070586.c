/*
 * A070586  a(n) = n^4 mod 43.
 * https://oeis.org/A070586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 43
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t mod = n % 43;          /* 0 <= mod < 43, fits in idx_t */
    seq_t m   = (seq_t)mod;      /* safe cast */
    seq_t r   = (m * m) % 43;    /* m^2 mod 43 */
    r         = (r * r) % 43;    /* m^4 mod 43 */
    return r;
}
