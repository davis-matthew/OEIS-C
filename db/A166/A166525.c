/*
 * A166525  a(n) = 10*n - a(n-1), with n>1, a(1)=1.
 * https://oeis.org/A166525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(10*n + 5 + 13*(-1)^n)
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence is defined only for n ≥ 1 */
        return 0;

    seq_t v = (seq_t)10 * (seq_t)n + 5;   /* 10*n + 5 */

    if (n & 1)           /* odd n : subtract 13  */
        v -= 13;
    else                 /* even n : add 13   */
        v += 13;

    return v / 2;        /* division by 2 is exact */
}
