/*
 * A076447  Let v(1)=v(2)=v(3)=1, v(n)=(-1)^n*sign(v(n-1)-v(n-2))*v(n-3), then
 *          a(n) =1+v(n).
 * https://oeis.org/A076447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * v(1) = v(2) = v(3) = 1 * v(n) = (-1)^n * sign(v(n-1) - v(n-2))
 *            * v(n-3) * a(n) = 1 + v(n) (offset 1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3u) return (seq_t)2;          /* a(1)=a(2)=a(3)=2 */

    /* v(n-3), v(n-2), v(n-1) */
    int v1 = 1;    /* v(1)  */
    int v2 = 1;    /* v(2)  */
    int v3 = 1;    /* v(3)  */
    int vi = 0;    /* v(n)  */

    for (idx_t i = 4u; i <= n; ++i)
    {
        int diff   = v3 - v2;                          /* v(n-1) - v(n-2) */
        int sgn    = (diff > 0) - (diff < 0);          /* sign function */
        int factor = (i & 1u) ? -1 : 1;                /* (-1)^i  */

        vi = factor * sgn * v1;                        /* v(n)    */

        /* shift for next iteration: v1 <- v2, v2 <- v3, v3 <- vi */
        v1 = v2;
        v2 = v3;
        v3 = vi;
    }

    return (seq_t)(1 + vi);   /* a(n) = 1 + v(n) */
}
