/*
 * A048031  Number of nonempty subsets of {1,2,...,n} in which exactly 3/4 of
 *          the elements are <= (n-3)/3.
 * https://oeis.org/A048031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t=1}^{⌊m/3⌋} C(m,3t)·C(n−m,t) , with m = ⌊(n−3)/3⌋
 *            Nonempty subsets of {1,…,n} in which exactly 3/4 of the elements
 *            are ≤ (n−3)/3
 * Range:     u8 n <= 20, u16 n <= 32, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t binom_mult(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        res = res * ((seq_t)N - K + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 3) return 0;

    idx_t m   = (n - 3) / 3;           /* floor((n-3)/3) */
    idx_t rest = n - m;                /* elements > m */
    idx_t max_t = m / 3;               /* largest t with 3t ≤ m */

    seq_t sum = 0;
    for (idx_t t = 1; t <= max_t; ++t) {
        seq_t c1 = binom_mult(m, 3 * t);       /* C(m,3t) */
        seq_t c2 = binom_mult(rest, t);        /* C(n-m,t) */
        sum += c1 * c2;
    }
    return sum;
}
