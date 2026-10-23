/*
 * A107255  a(n) = n^5 - 31*n + 31, with n*a(n) + n*( n - 1 )*31 = n^6.
 * https://oeis.org/A107255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 - 31*n + 31
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special: undefined for n < 1, but no harm leaving it */
    if (n == 0)
        return (seq_t)0;

    /* Compute in 128‑bit to avoid overflow on 64‑bit builds */
    unsigned __int128 nn   = (unsigned __int128)n;
    unsigned __int128 pow5 = nn * nn * nn * nn * nn;  /* n^5          */
    unsigned __int128 sub  = nn * (unsigned __int128)31; /* 31*n         */

    unsigned __int128 res128 = pow5 - sub + 31;   /* n^5 - 31*n + 31 */
    return (seq_t)res128;
}
