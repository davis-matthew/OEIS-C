/*
 * A055007  Number of nonnegative integer 4 X 4 matrices with no zero rows or
 *          columns and with sum of elements equal to n.
 * https://oeis.org/A055007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 20, u64 n <= 115, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A055007
   Number of nonnegative integer 4×4 matrices with no zero rows or columns
   and with sum of elements equal to n.
   Formula: a(n) = Σ_{k=0..4} (-1)^k C(4,k) m(4,4-k,n)
   where m(p,q,n) = Σ_{j=0..p} (-1)^j C(p,j) C((p-j)q + n - 1, n).   */

#include <stdint.h>

static const unsigned C4[5] = {1, 4, 6, 4, 1};

/* GCD for seq_t */
static seq_t gcd_seq(seq_t a, seq_t b) {
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Binomial coefficient C(N,R) for unsigned indices.
   N may be up to ~10015, R up to N.  Uses multiplicative
   algorithm with reduction by GCD to keep intermediate values
   integral and reasonably small. */
static seq_t binom(idx_t N, idx_t R) {
    if (R > N) return (seq_t)0;
    if (R == (idx_t)0 || R == N) return (seq_t)1;
    if (R > N - R) R = N - R;          /* use smaller k */

    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= R; ++i) {
        seq_t num = (seq_t)(N - R + i); /* numerator of term i */
        seq_t den = (seq_t)i;           /* denominator of term i */

        seq_t g1 = gcd_seq(num, den);
        num /= g1;
        den /= g1;

        seq_t g2 = gcd_seq(res, den);
        res /= g2;
        den /= g2;

        /* After reductions, den should be 1 */
        res *= num;
    }
    return res;
}

/* Compute m(4,q,n) = Σ_{j=0..4} (-1)^j C(4,j) C((4-j)q + n - 1, n) */
static seq_t m_4_q_n(idx_t q, idx_t n) {
    seq_t pos = (seq_t)0, neg = (seq_t)0;
    for (idx_t j = (idx_t)0; j <= (idx_t)4; ++j) {
        idx_t a = ((idx_t)4 - j) * q + n - (idx_t)1;
        if (n > a) continue;                 /* binomial would be zero */
        seq_t term = binom(a, n);
        term *= (seq_t)C4[j];
        if ( (j & (idx_t)1) == (idx_t)0 )
            pos += term;
        else
            neg += term;
    }
    return pos - neg;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Handle the trivial case n == 0 separately to avoid
       underflow in binomial arguments.  a(0) = 1. */
    if (n == (idx_t)0) return (seq_t)1;

    seq_t pos = (seq_t)0, neg = (seq_t)0;
    for (idx_t k = (idx_t)0; k <= (idx_t)4; ++k) {
        idx_t q = (idx_t)4 - k;
        seq_t m = m_4_q_n(q, n);          /* m(4,q,n) */
        seq_t prod = m * (seq_t)C4[k];
        if ( (k & (idx_t)1) == (idx_t)0 )
            pos += prod;
        else
            neg += prod;
    }
    return pos - neg;
}
