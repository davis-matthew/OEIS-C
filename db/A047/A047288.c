/*
 * A047288  Numbers that are congruent to {4, 6} mod 7.
 * https://oeis.org/A047288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(7*n/2) - (-1)^n (offset 1)
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset starts at 1; 0 is undefined */

    seq_t term = (seq_t)n * 7 / 2; /* floor(7*n/2) */
    if (n & 1)                    /* n odd: subtract (-1) = +1 */
        ++term;
    else                          /* n even: subtract (+1) = -1 */
        --term;
    return term;
}
