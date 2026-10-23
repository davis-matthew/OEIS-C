/*
 * A151642  Number of permutations of 4 indistinguishable copies of 1..n with
 *          exactly 4 adjacent element pairs in decreasing order.
 * https://oeis.org/A151642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 70^n - (4*n + 1)*35^n + binomial(4*n+1, 2)*15^n -
 *            binomial(4*n+1, 3)*5^n + binomial(4*n+1, 4)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* helper: fast exponentiation (base^exp) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}
 
/* helper: binomial coefficient C(n,k) computed multiplicatively */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}
 
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset is 1 */
    idx_t k = 4 * n + 1;
 
    seq_t pow70 = pow_seq((seq_t)70, n);
    seq_t pow35 = pow_seq((seq_t)35, n);
    seq_t pow15 = pow_seq((seq_t)15, n);
    seq_t pow5  = pow_seq((seq_t)5,  n);
 
    seq_t c2 = binom(k, 2);
    seq_t c3 = binom(k, 3);
    seq_t c4 = binom(k, 4);
 
    seq_t pos = pow70 + (c2 * pow15) + c4;
    seq_t neg = (pow35 * (seq_t)k) + (c3 * pow5);
 
    return (pos >= neg) ? (pos - neg) : 0;   /* should never be negative */
}
