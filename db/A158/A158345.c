/*
 * A158345  The number of pairs of independent outcomes when rolling an
 *          n-sided die. Or in other words, the number of pairs of proper
 *          subsets A,B of a set S, such that #A/#S * #B/#S = #(A \intersect
 *          B)/#S.
 * https://oeis.org/A158345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{a=0}^{n-1} Σ_{b=0}^{n-1} 
 *              [a*b % n == 0 and a*b/n ≤ min(a,b)] *
 *              C(n, a*b/n) * C(n - a*b/n, a - a*b/n) *
 *              C(n - a, b - a*b/n)
 *
 * where C(n,k) is the binomial coefficient.
 */

static seq_t binom(idx_t N, idx_t K)
{
    if (K > N)
        return 0;
    if (K > N - K)
        K = N - K;
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        res = res * (N - K + i);
        res = res / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t total = 0;
    for (idx_t a = 0; a < n; ++a) {
        for (idx_t b = 0; b < n; ++b) {
            idx_t prod = a * b;
            if (prod % n != 0)
                continue;
            idx_t gamma = prod / n;
            if (gamma > a || gamma > b)
                continue;
            seq_t term = binom(n, gamma);
            term = term * binom(n - gamma, a - gamma);
            term = term * binom(n - a, b - gamma);
            total += term;
        }
    }
    return total;
}
