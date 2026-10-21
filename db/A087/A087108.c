/*
 * A087108  This table shows the coefficients of combinatorial formulas needed
 *          for generating the sequential sums of p-th powers of binomial
 *          coefficients C(n,4). The p-th row (p>=1) contains a(i,p) for i=1
 *          to 4*p-3, where a(i,p) satisfies Sum_{i=1..n} C(i+3,4)^p = 5 *
 *          C(n+4,5) * Sum_{i=1..4*p-3} a(i,p) * C(n...
 * https://oeis.org/A087108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   * a(n) is produced by first locating the row index p and the *
 *            in‑row position i (1≤i≤4p-3) such that * S(p‑1) < n ≤ S(p) ,
 *            S(p)=p(2p‑1). * Then a(n)=a(i,p) where * * a(i,p) =
 *            Σ_{k=1..⌊(2i+1+(-1)^(i-1))/4⌋} * [
 *            C(i-1,2k-2)*C(i-2k+5,i-2k+1)^(p-1) *
 *            -C(i-1,2k-1)*C(i-2k+4,i-2k)^(p-1) ] . * * Offset:...
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 36, u64 n <= 100, u128 n <= 211,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Small helper: binomial coefficient nCk  (returns 0 if k>n).  */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;              /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res *= (seq_t)(n - k + i);         /* numerator part   */
        res /= (seq_t)i;                   /* exact division   */
    }
    return res;
}

/*  Small helper: exponentiation by squaring, exp is an idx_t (usually small).  */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/*  Sequence entry a(n)  (offset 1).  */
seq_t a(idx_t n)
{
    /* --- locate row p and in‑row index i --- */
    idx_t p = 1;
    idx_t S_prev = (idx_t)0;                     /* S(p-1) */
    idx_t S_curr = 1;                            /* S(p)   */
    while (S_curr < n)
    {
        ++p;
        S_prev = S_curr;
        S_curr = p * (2 * p - 1);                /* S(p) = p(2p-1) */
    }
    idx_t i = n - S_prev;                        /* 1‑based index in row */

    /* --- compute a(i,p) --- */
    seq_t result = (seq_t)0;

    /* Kmax = (i>>1) + (i&1)   (same as floor((2i+1+(-1)^(i-1))/4)) */
    idx_t Kmax = (i >> 1) + (i & 1);

    for (idx_t k = 1; k <= Kmax; ++k)
    {
        /* first term of the summand */
        seq_t coeff1 = binom_idx(i - 1, 2 * (k - 1));
        seq_t base1  = binom_idx(i - 2 * k + 5, i - 2 * k + 1);
        seq_t power1 = pow_seq(base1, p - 1);
        seq_t term1  = coeff1 * power1;

        /* second term of the summand */
        seq_t coeff2 = binom_idx(i - 1, 2 * k - 1);
        seq_t base2  = binom_idx(i - 2 * k + 4, i - 2 * k);
        seq_t power2 = pow_seq(base2, p - 1);
        seq_t term2  = coeff2 * power2;

        result += term1 - term2;               /* difference is non‑negative */
    }

    return result;
}
