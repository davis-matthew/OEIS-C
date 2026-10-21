/*
 * A219089  a(n) = floor((n + 1/2)^6).
 * https://oeis.org/A219089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 1/2)^6) = ((2n+1)^6) / 64
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 39, u64 n <= 1624,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic only for the intermediate product.
       It fits because (2n+1)^6 < 2^86 for all n that fit in seq_t. */
    unsigned __int128 m = (unsigned __int128)n * 2 + 1; /* m = 2n+1  */
    unsigned __int128 r = 1;
    for (int i = 0; i < 6; ++i) r *= m;           /* r = m^6       */
    r /= 64;                                       /* floor division by 64 */
    return (seq_t)r;
}
