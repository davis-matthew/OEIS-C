/*
 * A038247  Triangle whose (i,j)-th entry is binomial(i,j)*5^(i-j)*5^j.
 * https://oeis.org/A038247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 5^i * binom(i,j), where i = largest integer with i(i+1)/2
 *            ≤ n and j = n - i(i+1)/2 (the row and column of the triagular
 *            enumeration)
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow5_u(idx_t e)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (e)
    {
        if (e & 1ULL) result *= base;
        base *= base;
        e >>= 1ULL;
    }
    return result;
}
 
static seq_t binom_u(idx_t n, idx_t k)
{
    if (k > n) return 0ULL;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
        r = (seq_t)( ( (unsigned __int128)r * (n - k + i) ) / i );
    return r;
}
 
seq_t a(idx_t n)
{
    /* Find the row i such that i(i+1)/2 ≤ n < (i+1)(i+2)/2 */
    idx_t i   = 0;
    idx_t sum = 0;
    while (sum + (i + 1) <= n)
    {
        sum += i + 1;
        ++i;
    }
    idx_t j = n - sum;              /* column index within the row */
    seq_t val = pow5_u(i);          /* 5^i  */
    val *= binom_u(i, j);           /* * binom(i,j) */
    return val;
}
