/*
 * A158683  a(n) = 1024*n^2 - 32.
 * https://oeis.org/A158683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1024*n^2 - 32
 * Range:     u8 none, u16 n <= 8, u32 n <= 2048, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 1024 * n^2 - 32 in the type seq_t, ensuring that
       intermediate values stay within seq_t range (which may
       be 64‑bit or arbitrary precision) */
    seq_t t = (seq_t)n;          /* cast to seq_t */
    t = t * t;                   /* t = n^2 */
    t = t * (seq_t)1024;         /* t = 1024 * n^2 */
    t = t - (seq_t)32;           /* t = 1024*n^2 - 32 */
    return t;
}
