/*
 * A201248  Number of ways to place 7 non-attacking ferses on an n X n board.
 * https://oeis.org/A201248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^14/5040 - n^12/48 + n^11/30 + 673n^10/720 - 17n^9/6 * -
 *            1019n^8/48 + 197n^7/2 + 9772n^6/45 - 3443n^5/2 + 47n^4/4 * +
 *            74259n^3/5 - 1816352n^2/105 - 49376n + 90660 (for n ≥ 6) *
 *            Offset: 1 (a(1)=0, a(2)=0, a(3)=0, a(4)=216, a(5)=38070)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset values first */
    if (n < 4)               return (seq_t)0;
    if (n == 4)              return (seq_t)216;
    if (n == 5)              return (seq_t)38070;

    /* coefficients of 5040·a(n) for degrees 14 … 0   */
    static const int64_t coeff[15] = {
        1,          /* n^14 */
        0,          /* n^13 */
        -105,       /* n^12 */
        168,        /* n^11 */
        4711,       /* n^10 */
        -14280,     /* n^9 */
        -106995,    /* n^8 */
        496440,     /* n^7 */
        1094464,    /* n^6 */
        -8676360,   /* n^5 */
        59220,      /* n^4 */
        74853072,   /* n^3 */
        -87184896,  /* n^2 */
        -248855040, /* n   */
        456926400   /* constant */
    };

    /* compute powers of n: pow[0] = 1, pow[1] = n ... pow[14] = n^14 */
    seq_t pow[15];
    pow[0] = (seq_t)1;
    for (idx_t k = 1; k <= 14; ++k)
        pow[k] = pow[k - 1] * (seq_t)n;

    /* accumulate positive and negative contributions separately
       to avoid signed intermediates on unsigned seq_t types   */
    seq_t pos = (seq_t)0;
    seq_t neg = (seq_t)0;

    for (int i = 0; i < 15; ++i)
    {
        int64_t c = coeff[i];
        if (c >= 0)
            pos += pow[14 - i] * (seq_t)c;
        else
            neg += pow[14 - i] * (seq_t)(-c);
    }

    seq_t numerator = pos - neg;   /* always >= 0 for n >= 6 */
    return numerator / (seq_t)5040;
}
