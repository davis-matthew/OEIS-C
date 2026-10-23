/*
 * A184365  G.f.: eta(x) - x*eta'(x), where eta(x) is Dedekind's eta(q)
 *          function without the q^(1/24) factor.
 * https://oeis.org/A184365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 - Σ_{m≥1} (-1)^m * ( (m-1)(3m+2) + (m+1)(3m-2) * x^m )
 *            / 2 * x^{m(3m-1)/2} Expanding gives: a(n) = -(-1)^m *
 *            ((m-1)(3m+2))/2 if n = m(3m-1)/2 -(-1)^m * ((m+1)(3m-2))/2 if n
 *            = m(3m+1)/2 with a(0)=1
 * Range:     i8 n <= 95, i16 n <= 95, i32 n <= 95, i64 n <= 95, i128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    for (idx_t m = 1; ; ++m)
    {
        seq_t m_s = (seq_t)m;
        seq_t pent = m_s * (3 * m_s - 1) / 2;   /* m(3m-1)/2 */

        if (pent > n)
            break;

        seq_t sign = (m & 1) ? (seq_t)-1 : (seq_t)1;   /* (-1)^m */

        /* first component (plain x^pent) */
        seq_t val1 = ((m_s - 1) * (3 * m_s + 2)) / 2;
        if (pent == n)
            sum += -sign * val1;

        /* second component (x^(pent+m)) */
        seq_t val2 = ((m_s + 1) * (3 * m_s - 2)) / 2;
        seq_t pent_m = pent + m_s;
        if (pent_m == n)
            sum += -sign * val2;
    }

    return sum;
}
