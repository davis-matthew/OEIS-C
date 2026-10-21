/*
 * A064057  Eighth column of quintinomial coefficients.
 * https://oeis.org/A064057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3,3)*(n+14)*(n^3+15*n^2+116*n+120)/840
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    unsigned long long denom = 5040ULL;          /* 7! = 5040  */
    seq_t n_t = (seq_t)n;

    unsigned long long f1 = (unsigned long long)(n_t + 3);
    unsigned long long f2 = (unsigned long long)(n_t + 2);
    unsigned long long f3 = (unsigned long long)(n_t + 1);
    unsigned long long f4 = (unsigned long long)(n_t + 14);

    unsigned long long n_sq = (unsigned long long)n_t * (unsigned long long)n_t;
    unsigned long long poly = (unsigned long long)n_t * n_sq
                             + 15ULL * n_sq
                             + 116ULL * (unsigned long long)n_t
                             + 120ULL;

    /* gcd(a,b) for unsigned long long */
    unsigned long long a, b, t, g;

    /* factor f1 */
    a = denom;  b = f1;
    while (b != 0) { t = a % b; a = b; b = t; }
    g = a;  denom /= g;  f1 /= g;  res *= (seq_t)f1;

    /* factor f2 */
    a = denom;  b = f2;
    while (b != 0) { t = a % b; a = b; b = t; }
    g = a;  denom /= g;  f2 /= g;  res *= (seq_t)f2;

    /* factor f3 */
    a = denom;  b = f3;
    while (b != 0) { t = a % b; a = b; b = t; }
    g = a;  denom /= g;  f3 /= g;  res *= (seq_t)f3;

    /* factor f4 */
    a = denom;  b = f4;
    while (b != 0) { t = a % b; a = b; b = t; }
    g = a;  denom /= g;  f4 /= g;  res *= (seq_t)f4;

    /* polynomial factor */
    a = denom;  b = poly;
    while (b != 0) { t = a % b; a = b; b = t; }
    g = a;  denom /= g;  poly /= g;  res *= (seq_t)poly;

    if (denom > 1)
        res /= (seq_t)denom;

    return res;
}
