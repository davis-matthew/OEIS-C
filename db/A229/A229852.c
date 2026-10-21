/*
 * A229852  3*h^2, where h is an odd integer not divisible by 3.
 * https://oeis.org/A229852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*n^2 - 18*n + 3 for even n; a(n) = 27*n^2 - 36*n + 12
 *            for odd n
 * Range:     u8 n <= 3, u16 n <= 49, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1, but harmless */

    seq_t s = (seq_t)n;
    if ((n & 1) == 0) {            /* even */
        return (27 * s * s) - (18 * s) + 3;
    } else {                       /* odd */
        return (27 * s * s) - (36 * s) + 12;
    }
}
