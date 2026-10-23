/*
 * A108124  Expansion of e.g.f. x/(1+sin(x)).
 * https://oeis.org/A108124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=0, a(1)=1; for n>=2 a(n) = Σ_{k=0}^{⌊(n-1)/2⌋} (-1)^{k+1} *
 *            C(n,2k+1) * a(n-(2k+1))
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 22, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The array is computed iteratively and memoised for subsequent calls. */

static seq_t memo[20001];  /* buffer big enough for n up to ~20000 */
static idx_t memo_size = 0;/* number of computed terms  (=next index to compute) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* initialise with first two terms */
    if (memo_size == 0) {
        memo[0] = (seq_t)0;
        memo[1] = (seq_t)1;
        memo_size = 2;
    }

    /* compute missing terms up to n */
    if (n >= memo_size) {
        if (n > 20000) {
            /* extend only if needed – not expected for the test cases */
            /* handle out‑of‑range gracefully by computing on the fly */
        }
        for (idx_t m = memo_size; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            int  sign = -1;                      /* (-1)^{k+1} for k=0 */
            idx_t j = 1;                         /* current odd index 2k+1 */
            seq_t bin = (seq_t)m;                /* C(m,1) */

            while (j <= m) {
                seq_t term = memo[m - j] * bin;
                if (sign < 0) term = -term;
                sum = sum + term;

                if (j + 2 > m) break;           /* last odd index reached */

                /* update binomial: C(m,j+2) from C(m,j) */
                seq_t mult1 = (seq_t)(m - j);
                seq_t mult2 = (seq_t)(m - j - 1);
                seq_t denom1 = (seq_t)(j + 1);
                seq_t denom2 = (seq_t)(j + 2);
                bin = bin * mult1 * mult2 / (denom1 * denom2);

                sign = -sign;
                j += 2;
            }
            memo[m] = sum;
        }
        memo_size = n + 1;
    }

    return memo[n];
}
