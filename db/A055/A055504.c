/*
 * A055504  a(n) = n*(n-1)*(n-2)*(n-3)*(n-4)*(2*n-1)/72.
 * https://oeis.org/A055504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3)*(n-4)*(2*n-1)/72
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 75, u64 n <= 2955,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) return 0;                     /* terms 0..4 are 0 */

    /* compute with unsigned __int128 to avoid overflow during intermediate
       multiplications; the result is cast back to seq_t.
       This works for all builds: 64‑bit and arbitrary‑precision. */
    unsigned __int128 val = (unsigned __int128)n;
    val *= (unsigned __int128)(n-1);
    val *= (unsigned __int128)(n-2);
    val *= (unsigned __int128)(n-3);
    val *= (unsigned __int128)(n-4);
    val *= ((unsigned __int128)2 * n - 1);     /* 2*n-1 computed in 128‑bit */
    val /= 72;

    return (seq_t)val;                         /* safe conversion to seq_t */
}
