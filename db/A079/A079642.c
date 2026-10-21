/*
 * A079642  Matrix product of unsigned Stirling1-triangle |A008275(n,k)| and
 *          Stirling1-triangle A008275(n,k).
 * https://oeis.org/A079642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   compute a(n) = T(r,k) where T(r,k)=∑_{i=k}^{r} |s(r,i)|·s(i,k) *
 *            with s(·,·) the signed Stirling numbers of the first kind and
 *            |s(r,i)| * their absolute values. The sequence is flattened
 *            row‑by‑row. A value * is returned for all n≥1; behaviour for n<1
 *            is undefined
 * Range:     u8 n <= 21, u16 n <= 36, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers are allowed. */
#include <stdlib.h>

/* Maximum row we need to handle for n up to about 10 000.
 * The smallest r that satisfies r(r+1)/2 ≥ 10 000 is 141,
 * so 200 is an ample safety margin. */
#define MAX_ROW 200

/* Storage for unsigned Stirling numbers of the first kind.
 * c[n][k] (0 ≤ n,k ≤ MAX_ROW) holds |s(n,k)| as seq_t. */
static seq_t *c = NULL;

/* Allocate and compute the unsigned Stirling numbers up to MAX_ROW.
 * Uses the recurrence |s(n,k)| = |s(n-1,k-1)| + (n-1)·|s(n-1,k)|. */
static void init_stirling(void)
{
    if (c != NULL)
        return;                           /* already initialised */

    size_t size = (MAX_ROW + 1) * (MAX_ROW + 1);
    c = (seq_t *)malloc(size * sizeof(seq_t));
    /* initialise all entries to zero */
    for (size_t i = 0; i < size; ++i)
        c[i] = 0;

    /* base case: |s(0,0)| = 1 */
    c[0] = 1;

    /* compute rows 1..MAX_ROW */
    for (idx_t n = 1; n <= MAX_ROW; ++n) {
        for (idx_t k = 1; k <= n; ++k) {
            /* index into the flattened array */
            size_t idx_nm1_k_1 = ((size_t)(n - 1) * (MAX_ROW + 1)) + (k - 1);
            size_t idx_nm1_k   = ((size_t)(n - 1) * (MAX_ROW + 1)) + k;
            size_t idx_nk      = ((size_t)n * (MAX_ROW + 1)) + k;

            seq_t term1 = c[idx_nm1_k_1];
            seq_t term2 = c[idx_nm1_k] * (seq_t)(n - 1);

            c[idx_nk] = term1 + term2;
        }
    }
}

/* Fetch |s(n,k)| from the pre‑computed table. */
static inline seq_t abs_s(idx_t n, idx_t k)
{
    return c[((size_t)n * (MAX_ROW + 1)) + k];
}

/* Return the n-th term of A079642. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined, but safe guard */

    init_stirling();

    /* Determine the row r and column k such that the overall
     * index n falls in row r (1‑based) and column k (1‑based).
     * The cumulative count of entries up to row r is r(r+1)/2. */
    idx_t r = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t k = n - (cum - r);           /* 1 ≤ k ≤ r */

    /* Accumulate the alternating sum:
     *   sign(i,k) = (-1)^(i-k) = + for even (i-k), – for odd (i-k) */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t i = k; i <= r; ++i) {
        seq_t prod = abs_s(r, i) * abs_s(i, k);
        /* even or odd difference? idx_t is unsigned, so use modulo 2. */
        if ((i - k) & 1)
            sum_neg += prod;            /* odd → negative term */
        else
            sum_pos += prod;            /* even → positive term */
    }

    /* The result is non‑negative. */
    seq_t res = sum_pos;
    if (sum_pos >= sum_neg)
        res -= sum_neg;
    else
        res = 0;                      /* should not happen */

    return res;
}
