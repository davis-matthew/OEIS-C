/*
 * A082970  Number of permutations of length n containing 2 occurrences of
 *          132.
 * https://oeis.org/A082970
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(2*n-6, n-2)*(n^3 + 17*n^2 - 80*n + 80)/(2*n*(n-1))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 4  (a(4) is the first defined term) */

#include <stdint.h>

/* Euclidean gcd for seq_t types.  Works for all integer types that support
   the standard arithmetic operators. */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Multiply the current binomial result by a fraction num/den with exact division.
   The fraction is first reduced by gcd reductions to prevent overflow. */
static seq_t binom_reduce_mul(seq_t res, seq_t num, seq_t den)
{
    /* Cancel gcd between numerator and denominator */
    seq_t g = seq_gcd(num, den);
    num /= g;
    den /= g;

    /* Cancel gcd between current result and remaining denominator */
    g = seq_gcd(res, den);
    res /= g;
    den /= g;

    /* Multiply and then divide; division is exact */
    res = res * num;
    res = res / den;
    return res;
}

/* Compute binomial coefficient C(N, K) using multiplicative formula with
   gcd reductions to keep intermediate values small.  Assumes 0 <= K <= N. */
static seq_t binom_uint64(idx_t N, idx_t K)
{
    if (K > N) return (seq_t)0;
    if (K > N - K) K = N - K;          /* use symmetry */

    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= K; ++i) {
        seq_t num = (seq_t)(N - K + i); /* numerator part */
        seq_t den = (seq_t)i;           /* denominator part */
        result = binom_reduce_mul(result, num, den);
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Undefined for n < 4 – return 0 */
    if (n < (idx_t)4) return (seq_t)0;

    /* Promote index to seq_t for arithmetic */
    seq_t ns = (seq_t)n;

    /* Compute binomial coefficient C(2*n-6, n-2) */
    idx_t N = (idx_t)(2 * ns - 6);
    idx_t K = (idx_t)(ns - 2);
    seq_t binom = binom_uint64(N, K);

    /* Polynomial part: n^3 + 17*n^2 - 80*n + 80 */
    seq_t poly = ns * ns * ns            /* n^3 */
              + (seq_t)17 * ns * ns      /* +17*n^2 */
              - (seq_t)80 * ns           /* -80*n */
              + (seq_t)80;               /* +80 */

    /* Denominator part: 2*n*(n-1) */
    seq_t den = (seq_t)2 * ns * (ns - (seq_t)1);

    /* Final result */
    return binom * poly / den;
}
