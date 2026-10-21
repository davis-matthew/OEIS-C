/*
 * A211483  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and (w+n)^2=x+y.
 * https://oeis.org/A211483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = (2*n+1)*(floor(sqrt(2*n))+1) * -
 *            floor(sqrt(2*n))*(floor(sqrt(2*n))+1)*(2*floor(sqrt(2*n))+1)/6 *
 *            where floor(sqrt(x)) is the greatest integer k with k*k ≤ x
 * Range:     u8 n <= 12, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = x;
    while (low < high)
    {
        idx_t mid = (low + high + 1) >> 1;
        if ((__int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* 2*n  and  floor(sqrt(2*n))  are computed using idx_t arithmetic  */
    idx_t two_n = n + n;
    idx_t m = isqrt_u64(two_n);          /* floor(sqrt(2*n)) */

    /* Convert to seq_t for the arithmetic, casting indices first  */
    seq_t two_n_seq = (seq_t)n * ((seq_t)2);          /* 2*n */
    seq_t m_seq = (seq_t)m;
    seq_t term1 = (two_n_seq + (seq_t)1) * (m_seq + (seq_t)1);

    /* m*(m+1)*(2*m+1)/6  */
    seq_t sumSquares = m_seq * (m_seq + (seq_t)1);
    sumSquares = sumSquares * ((seq_t)2 * m_seq + (seq_t)1);
    sumSquares = sumSquares / (seq_t)6;

    seq_t result = term1 - sumSquares;
    return result;
}
