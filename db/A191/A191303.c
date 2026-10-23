/*
 * A191303  An infinite sequence of 4-digit half-palindromes.
 * https://oeis.org/A191303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (2n+2)(14n+9)^3 + (3n+3)(14n+9)^2 + (5n+3)(14n+9) +
 *            (2n+1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;               /* convert index to seq_t */
    seq_t base = 14 * nn + 9;          /* base b = 14n + 9 */

    seq_t base2 = base * base;         /* (14n+9)^2 */
    seq_t base3 = base2 * base;        /* (14n+9)^3 */

    seq_t term1 = (2 * nn + 2) * base3;    /* (2n+2)(14n+9)^3 */
    seq_t term2 = (3 * nn + 3) * base2;    /* (3n+3)(14n+9)^2 */
    seq_t term3 = (5 * nn + 3) * base;     /* (5n+3)(14n+9)  */
    seq_t term4 = 2 * nn + 1;              /* 2n+1 */

    return term1 + term2 + term3 + term4;
}
