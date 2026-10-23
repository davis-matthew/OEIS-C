/*
 * A101861  a(n) = n*(n+5)*(50+45*n+n^2)/24.
 * https://oeis.org/A101861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+5)*(50+45*n+n^2)/24
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is offset 1. No need to handle n==0, but
       returning 0 for safety does not hurt. */
    if (n == 0) return (seq_t)0;

    /* Compute using unsigned 128‑bit arithmetic to avoid
       intermediate overflow when seq_t is small (uint8_t,
       uint16_t, …).  The final result is cast to seq_t, which
       may be 8, 16, 32, 64, 128 bits or an arbitrary‑precision
       integer class. */
    unsigned __int128 nn  = (unsigned __int128)n;
    unsigned __int128 tmp = nn * (nn + 5);                       /* n*(n+5) */
    tmp  = tmp * (nn * nn + (unsigned __int128)45 * nn + 50);    /* * (n^2+45n+50) */
    tmp /= 24;

    return (seq_t)tmp;
}
