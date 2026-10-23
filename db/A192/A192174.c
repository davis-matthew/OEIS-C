/*
 * A192174  Triangle T(n,k) of the coefficients [x^(n-k)] of the polynomial
 *          p(0,x)=-1, p(1,x)=x and p(n,x) = x*p(n-1,x) - p(n-2,x) in row n,
 *          column k.
 * https://oeis.org/A192174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 71, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A192174 – triangle coefficients T(n,k) with k even:
 *  T(n,2j) = (-1)^j [ binomial(n-j,j) – 2*binomial(n-j-1,j-1) ]
 *  T(n,k)=0 for k odd,   T(0,0) = -1.
 *  The sequence a(n) is the flattened triangle row‑by‑row (row 0,1,2,…).  */

/* multiplicative binomial – safe for small n (≤ ~200) */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;          /* use smaller k for speed */

    seq_t r = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* r = r * (n-k+i) / i  – each step integer */
        r = r * (seq_t)(n - k + i);
        r = r / (seq_t)i;
    }
    return r;
}

/* coefficient T(n,k) using the closed formula above */
static seq_t triangle_coeff(idx_t n, idx_t k)
{
    if (n == 0 && k == 0) return (seq_t)-1;

    if (k & 1u)                 /* k odd -> zero */
        return (seq_t)0;

    idx_t j = k >> 1;                       /* j = k/2 */
    seq_t term1 = binom(n - j, j);
    seq_t term2 = (j > 0) ? binom(n - j - 1, j - 1) : (seq_t)0;

    seq_t diff = term1 - (term2 + term2);   /* term1 - 2*term2 */

    if (j & 1u)                    /* apply (-1)^j */
        diff = -diff;
    return diff;
}

/* find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2  */
static idx_t find_row(idx_t n)
{
    idx_t r = 0;
    while ((r + 1u) * (r + 2u) / 2u <= n)
        ++r;
    return r;
}

/* public API: a(n) for A192174 */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t k = n - r * (r + 1u) / 2u;   /* column index within row r */
    return triangle_coeff(r, k);
}
