/*
 * A121626  Real part of (1 + n*i)^n, where i=sqrt(-1).
 * https://oeis.org/A121626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{j=0..floor(n/2)} binomial(n,2j) * n^(2j) * (-1)^j
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute a(n) = Re((1 + n*i)^n) = Σ_{j=0}^{⌊n/2⌋} C(n,2j) * n^{2j} * (-1)^j */
    seq_t sum = 0;                      /* running sum */
    seq_t comb = 1;                     /* C(n,0) */
    seq_t pow_n2 = 1;                   /* n^{0} */
    seq_t sign = 1;                     /* (-1)^j, start with j=0 */
    seq_t nn = (seq_t)n;                /* promote n to seq_t */
    seq_t n_squared = nn * nn;          /* n^2, used to update pow_n2 */
    idx_t j = 0;
    idx_t max_j = n / 2;                /* floor(n/2) */

    while (1)
    {
        /* Add current term: sign * comb * pow_n2 */
        if (sign > 0)
            sum += comb * pow_n2;
        else
            sum -= comb * pow_n2;

        if (j == max_j)
            break;                      /* last term processed */

        /* Prepare next combinatorial factor:
           C(n, 2(j+1)) = C(n, 2j) * (n-2j)*(n-2j-1)/((2j+1)*(2j+2)) */
        seq_t two_j = (seq_t)(2 * j);
        seq_t num   = comb;
        num *= (nn - two_j);
        num *= (nn - two_j - 1);
        seq_t denom = (two_j + 1) * (two_j + 2);
        comb = num / denom;             /* exact division */

        /* Update power factor: n^{2(j+1)} = n^{2j} * n^2 */
        pow_n2 *= n_squared;

        /* Toggle sign for next term */
        sign = -sign;
        ++j;
    }
    return sum;
}
