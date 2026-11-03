/*
 * A228636  Number of s in {1,...,n}^n having shortest run with the same value
 *          of length 10.
 * https://oeis.org/A228636
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f_min(10)(n) - f_min(11)(n) where f_min(L)(n) =
 *            Σ_{k=1}^{⌊n/L⌋} C(n - L*k + k - 1, k - 1) * n * (n-1)^(k-1)
 * Range:     u8 n <= 19, u16 n <= 30, u32 n <= 51, u64 n <= 87, u128 n <= 155,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 168 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* binomial coefficient C(N, R) for N >= R >= 0, using multiplicative formula */
static seq_t binom_comb(seq_t N, int R)
{
    seq_t result = 1;
    for (int i = 1; i <= R; ++i) {
        seq_t numerator = N - (seq_t)(R - i); /* N - R + i */
        result *= numerator;
        result /= (seq_t)i;
    }
    return result;
}

/* f_min(L)(n) for L >= 1 */
static seq_t f_min(idx_t n, int L)
{
    seq_t total = 0;
    idx_t maxk = n / L;            /* maximum number of runs */
    if (maxk == 0) return 0;

    seq_t nseq    = (seq_t)n;
    seq_t nminus1 = nseq - 1;
    seq_t power   = 1;              /* (n-1)^(k-1) */

    for (idx_t k = 1; k <= maxk; ++k) {
        int r = (int)(k - 1);       /* combinatorial parameter */
        /* m = n - L*k + k - 1 = n - (L-1)*k - 1 */
        seq_t m = nseq - (seq_t)L * k + (seq_t)k - 1;
        seq_t bin = binom_comb(m, r);

        seq_t term = bin;
        term *= nseq;      /* multiply by n */
        term *= power;     /* multiply by (n-1)^(k-1) */

        total += term;

        power *= nminus1;  /* advance power for next iteration */
    }
    return total;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 10) return 0;          /* offset behaviour */

    seq_t f10 = f_min(n, 10);
    seq_t f11 = f_min(n, 11);
    return f10 - f11;
}
