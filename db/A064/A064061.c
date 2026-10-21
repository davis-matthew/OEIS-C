/*
 * A064061  Eighth column of Catalan triangle A009766.
 * https://oeis.org/A064061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*binomial(n+14,6)/7
 * Range:     u8 none, u16 n <= 3, u32 n <= 69, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;                 /* will hold binomial(n+14,6)   */
    idx_t  m   = n + 14;           /* n+14  */

    /* compute C(m,6) multiplicatively to avoid large intermediates   */
    for (idx_t i = 1; i <= 6; ++i)
    {
        /* multiply by the corresponding numerator, then divide by i */
        res = res * (seq_t)(m - 6 + i) / (seq_t)i;
    }

    /* incorporate the remaining factors (n+1) and divide by 7         */
    res = res * (seq_t)(n + 1);
    res = res / (seq_t)7;

    return res;
}
