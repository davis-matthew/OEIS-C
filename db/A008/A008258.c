/*
 * A008258  Coordination sequence T1 for Moganite.
 * https://oeis.org/A008258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*floor(n/3)^2 + * 2 if n mod 3 = 0 and n>0, *
 *            14*floor(n/3)+4 if n mod 3 = 1 and n>1, * 30*floor(n/3)+12 if n
 *            mod 3 = 2 and n>2. * With the special cases a(0)=1, a(1)=4,
 *            a(2)=10
 * Range:     u8 n <= 10, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the three exceptional small indices */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)10;

    /* for n >= 3 we can apply the piecewise quadratic formulas directly */
    idx_t m = n / 3;               /* floor(n/3) */
    seq_t M = (seq_t)m;
    seq_t res;
    switch (n % 3) {
        case 0:  /* n = 3*m  with m > 0 */
            res = 22 * M * M + (seq_t)2;
            break;
        case 1:  /* n = 3*m + 1  with m > 0 */
            res = 22 * M * M + 14 * M + (seq_t)4;
            break;
        case 2:  /* n = 3*m + 2  with m > 0 */
            res = 22 * M * M + 30 * M + (seq_t)12;
            break;
    }
    return res;
}
