/*
 * A083559  Nearest integer to 1/(Sum_{k>=n} 1/k^4).
 * https://oeis.org/A083559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((12*n^3 - 18*n^2 + 15*n - 3)/4) for n > 3 with
 *            a(1)=1, a(2)=12, a(3)=50
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 12;
    if (n == 3) return 50;

    /* use a wider integer type for safe intermediate calculation */
    __int128 m = n;                     /* promote n to 128‑bit */
    __int128 num = 12 * m * m * m
                 - 18 * m * m
                 + 15 * m
                 - 3;
    return (seq_t)(num / 4);
}
