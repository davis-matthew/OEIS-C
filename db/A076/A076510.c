/*
 * A076510  Expansion of 3*(1+2*x+6 x^2)/(1-24*x^3).
 * https://oeis.org/A076510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b[r] * 24^{floor((n-1)/3)}, where r = (n-1)%3 and b[0]=3,
 *            b[1]=6, b[2]=18
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for the sequence, but defined for safety */

    idx_t k = (n - 1) / 3;       /* number of 24‑multiplications */
    idx_t r = (n - 1) % 3;       /* determines the base value 3,6,18 */

    seq_t result;
    if (r == 0)
        result = 3;
    else if (r == 1)
        result = 6;
    else                        /* r == 2 */
        result = 18;

    for (idx_t i = 0; i < k; ++i)
        result *= 24;

    return result;
}
