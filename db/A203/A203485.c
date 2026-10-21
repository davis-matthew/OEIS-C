/*
 * A203485  Number of n X 2 0..2 arrays with row sums equal and column sums
 *          unequal to adjacent columns.
 * https://oeis.org/A203485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*2^n + 3^n - (if n even: 2*binom(n, n/2) + Cn) else: Cn
 *            where Cn is the coefficient of t^n in (1+t+t^2)^n (i.e. the
 *            number of arrays for r = 2 with equal row sum n and column sums
 *            equal). The implementation uses an O(n) recurrence to compute Cn
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* compute binomial coefficient C(n, k) for n,k up to idx_t */
static seq_t binom_idx(idx_t n, idx_t k) {
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - i + 1) / (seq_t)i;
    }
    return res;
}

/* fast exponentiation: base^exp */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* a(idx_t n) returns the nth term of OEIS A203485 */
seq_t a(idx_t n) {
    /* basic powers */
    seq_t pow2 = pow_seq(2, n);
    seq_t pow3 = pow_seq(3, n);

    /* coefficient Cn for r=2 */
    seq_t waysR2 = 1;              /* k = 0 term */
    seq_t C1 = 1;                  /* binom(n,0) */
    seq_t a_k = 1;                 /* binom(n,0) <-- a_k = C(n-k, k) when k=0 */
    idx_t half = n / 2;
    for (idx_t k = 1; k <= half; ++k) {
        /* C1 = C(n, k) */
        C1 = C1 * (n - k + 1) / (seq_t)k;

        /* a_k = C(n - k, k) using recurrence:
           a_k = a_{k-1} * (n - 2k + 2) * (n - 2k + 1) / (k * (n - k + 1))
        */
        seq_t num = a_k * (seq_t)(n - 2 * k + 2) * (seq_t)(n - 2 * k + 1);
        seq_t denom = (seq_t)k * (seq_t)(n - k + 1);
        a_k = num / denom;

        waysR2 += C1 * a_k;
    }

    /* start with the base count: both r=1 and r=3 contribute 2*2^n */
    seq_t result = pow2 + pow2 + pow3;  /* 2*2^n + 3^n */

    if ((n & 1) == 0) { /* n even: subtract from r=1 and r=3 */
        seq_t binomHalf = binom_idx(n, n / 2);
        result -= binomHalf + binomHalf;  /* 2*binom(n, n/2) */
    }

    /* subtract the r=2 forbidden cases */
    result -= waysR2;

    return result;
}
