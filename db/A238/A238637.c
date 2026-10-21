/*
 * A238637  Number of partitions of 10^n into parts that are at most 10.
 * https://oeis.org/A238637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^(10^n) in ∏_{j=1..10} 1/(1-x^j)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"

/* lcm(1..10) = 2520 – period of the quasi‑polynomial */
#define PERIOD 2520
/* dp and value tables are computed once and reused */
#define MAXM 25199                /* up to 10^n for n=0..4 */

static bool initialized = false;
static seq_t dp[MAXM + 1];                 /* partitions up to MAXM   */
static seq_t values[PERIOD][10];            /* values[r][k] = a(2520*k+r) */
static seq_t denom_abs[10];                 /* |denominator| for Lagrange terms */
static int   sign_coef[10];                 /* sign of each term (-1 or 1)   */

/* initialise tables on first call */
static void init_tables(void)
{
    /* 1. Compute dp[0..MAXM] – partitions with parts ≤ 10 */
    dp[0] = 1;
    for (int part = 1; part <= 10; ++part) {
        for (size_t m = part; m <= MAXM; ++m) {
            dp[m] += dp[m - part];
        }
    }

    /* 2. Fill residue table values[r][k] for 0≤r<PERIOD, 0≤k<10 */
    for (size_t r = 0; r < PERIOD; ++r) {
        for (size_t k = 0; k < 10; ++k) {
            values[r][k] = dp[(size_t)(r + PERIOD * k)];
        }
    }

    /* 3. Precompute denominators for Lagrange interpolation */
    seq_t fact[10];
    fact[0] = 1;
    for (int i = 1; i < 10; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    for (int i = 0; i < 10; ++i) {
        denom_abs[i] = fact[i] * fact[9 - i];
        /* sign = (-1)^{9-i} */
        sign_coef[i] = ((9 - i) & 1) ? -1 : 1;
    }

    initialized = true;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (!initialized) init_tables();

    /* compute M = 10^n */
    seq_t M = pow10(n);

    /* 1. Small M – return directly from table */
    if (M <= (seq_t)MAXM) {
        return dp[(size_t)M];
    }

    /* 2. Quotient K = floor(M/2520) and remainder r = M mod 2520 */
    seq_t K = M / PERIOD;                  /* K is floor(M/2520)  */
    uint64_t r = (uint64_t)(M % PERIOD);   /* r fits in 64 bits    */

    /* 3. Compute P = ∏_{t=0}^9 (K - t) */
    seq_t P = 1;
    for (int t = 0; t < 10; ++t) {
        P *= (K - (seq_t)t);
    }

    /* 4. Evaluate Lagrange formula for residue r */
    seq_t pos = 0, neg = 0;
    for (int i = 0; i < 10; ++i) {
        seq_t i_seq = (seq_t)i;
        /* If K == i, the term vanishes (numerator zero) */
        if (K == i_seq) {
            continue;
        }

        /* numerator_i = P / (K - i) */
        seq_t numerator = P / (K - i_seq);

        /* multiply by stored value a(2520*i + r) */
        numerator *= values[r][i];

        /* divide by the denominator (absolute value) */
        numerator /= denom_abs[i];

        /* accumulate with sign */
        if (sign_coef[i] >= 0) {
            pos += numerator;
        } else {
            neg += numerator;
        }
    }

    /* final result – non‑negative */
    return (pos >= neg) ? (pos - neg) : 0;
}
