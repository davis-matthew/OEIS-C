/*
 * A069906  Number of pentagons that can be formed with perimeter n. In other
 *          words, number of partitions of n into five parts such that the sum
 *          of any four is more than the fifth.
 * https://oeis.org/A069906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (11*n^4 + 100*n^3 + 200*n^2 - 960*n + 14255)/46080
 *            + ((n mod 2)*(2*n^3 + 15*n^2 + 120*n))/4608 + (((n^2 + 3*n) mod
 *            4)*n)/128 )
 * Range:     u8 n <= 29, u16 n <= 126, u32 n <= 2056, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit unsigned integers for intermediate calculations 
       to avoid overflow on typical 64‑bit seq_t builds.
       The result is still returned as a seq_t value. */
    unsigned __int128 N = n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;

    /* Numerator of the first fraction (may be negative before adding the constant) */
    __int128   A_signed = (__int128)11 * n4
                        + (__int128)100 * n3
                        + (__int128)200 * n2
                        - (__int128)960 * N
                        + 14255;
    unsigned __int128 A = (unsigned __int128)A_signed;

    /* Second term: only present when n is odd */
    unsigned __int128 B = (n & 1U)
                        ? (2 * n3 + 15 * n2 + 120 * N)
                        : 0;

    /* Third term involves (n^2 + 3*n) mod 4 */
    unsigned __int128 r   = (n2 + 3 * N) & 3U;        /* modulo 4 */
    unsigned __int128 C   = r * N;                    /* r * n */

    /* Combine all over the common denominator 46080 */
    unsigned __int128 num = A + B * 10U + C * 360U;   /* 46080 = 4608*10 = 128*360 */
    return (seq_t)(num / 46080U);
}
