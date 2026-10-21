/*
 * A219085  a(n) = floor((n + 1/2)^3).
 * https://oeis.org/A219085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 1/2)^3) = floor((2n+1)^3 / 8)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1624, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow for n up to about 5·10¹²
       (sufficient for all tests on 8/16/32/64‑bit builds).  */
    unsigned __int128 x = (unsigned __int128)n * 2 + 1;   /* 2n+1 */
    unsigned __int128 cube = x * x * x;                   /* (2n+1)^3 */
    cube >>= 3;                                          /* divide by 8 */
    return (seq_t)cube;
}
