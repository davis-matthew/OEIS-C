/*
 * A090771  Numbers that are congruent to {1, 9} mod 10.
 * https://oeis.org/A090771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 3*(-1)^n - 5)/2
 * Range:     u8 n <= 25, u16 n <= 6572, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* value below offset is undefined */
    seq_t num = (seq_t)10 * (seq_t)n; /* 10*n */

    /* add or subtract 3 according to (-1)^n */
    if (n & (idx_t)1)                /* n is odd: (-1)^n = -1 → subtract 3 */
        num -= (seq_t)3;
    else                             /* n is even: (-1)^n = +1 → add 3 */
        num += (seq_t)3;

    num -= (seq_t)5;                 /* subtract 5 */
    return num / (seq_t)2;           /* divide by 2 */
}
