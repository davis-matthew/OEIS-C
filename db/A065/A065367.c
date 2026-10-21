/*
 * A065367  Replace 3^k with (-3)^k in balanced ternary expansion of n.
 * https://oeis.org/A065367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ a_k * (-3)^k  where the digits a_k (k≥0)
 * are the balanced ternary digits of n (each a_k ∈ {−1,0,1}).
 */

seq_t a(idx_t n)
{
    seq_t result = 0;          /* the value of a(n)                      */
    seq_t power  = 1;          /* current 3^k                           */
    int    k     = 0;          /* position of the current digit         */
    idx_t  m     = n;          /* working copy of the input             */

    while (m != 0)
    {
        int rem = m % 3;       /* remainder modulo 3                   */
        m /= 3;                /* shift right one ternary position     */
        int digit = rem;       /* balanced ternary digit (initial)     */

        if (digit == 2)        /* replace 2 with −1 and carry 1        */
        {
            digit = -1;
            m += 1;
        }

        int sign = (k & 1) ? -1 : 1;   /* (-1)^k                      */
        result += (seq_t)(digit * sign) * power;  /* a_k*(-1)^k*3^k */

        power *= 3;               /* advance 3^k                         */
        k++;                      /* next digit position                */
    }

    return result;
}
