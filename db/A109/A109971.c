/*
 * A109971  Inverse of Riordan array (1,x(1-x)^2), A109970.
 * https://oeis.org/A109971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r, c) where r = row_of_index(n) satisfies r(r+1)/2 ≤ n
 *            < (r+1)(r+2)/2, c = n - r(r+1)/2, and the triangle entry T(r, c)
 *            = binomial(3r - c, r - c) * 2c / (3r - c) (T(0,0)=1)
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_uint64(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;           /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= K; ++i) {
        result = result * (seq_t)(N - K + i);
        result = result / (seq_t)i;     /* exact division */
    }
    return result;
}

/* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
static idx_t row_of_index(idx_t n)
{
    idx_t r = 0;
    while (r * (r + 1) / 2 <= n) ++r;
    return r - 1;
}

/* Triangle entry T(r, c).  Special case r=0,c=0 -> 1; c=0 -> 0. */
static seq_t compute_T(idx_t r, idx_t c)
{
    if (r == 0 && c == 0) return 1;
    if (c == 0) return 0;
    idx_t N = 3 * r - c;
    idx_t K = r - c;
    seq_t val = binom_uint64(N, K);
    val = val * (seq_t)(2 * c);
    val = val / (seq_t)N;
    return val;
}

seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);                       /* row index  */
    idx_t start = r * (r + 1) / 2;                   /* first index of this row  */
    idx_t c = n - start;                             /* column index in the row   */
    return compute_T(r, c);
}
