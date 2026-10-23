/*
 * A008387  Coordination sequence for A_6 lattice.
 * https://oeis.org/A008387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n*(11*n^4 + 35*n^2 + 14)/10 for n > 0; a(0) = 1
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 55, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sn  = (seq_t)n;          /* n as seq_t */
    seq_t n2  = sn * sn;           /* n^2 */
    seq_t n4  = n2 * n2;           /* n^4 */
    seq_t t   = 11 * n4 + 35 * n2 + 14; /* 11*n^4 + 35*n^2 + 14 */
    seq_t num = 7 * sn * t;        /* 7*n*t */
    return num / 10;               /* divide by 10 (exact) */
}
