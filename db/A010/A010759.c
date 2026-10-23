/*
 * A010759  Sum along upward diagonal of Pascal triangle from halfway point.
 * https://oeis.org/A010759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=floor((n+2)/4)..floor(n/2)} binomial(n-k, k)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS sequence A010759.
 * It computes the required binomial coefficients in a safe
 * multiplicative manner that avoids overflow on 64‑bit builds
 * and works with arbitrary‑precision types as well. */

#include <stdint.h>

/* Small helper to compute the greatest common divisor of two seq_t values.
 * Works for 8,16,32,64,128 bit unsigned integers as well as for built‑in
 * arbitrary‑precision integer classes that support the % operator. */
static inline seq_t _gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute binomial(n,k) for idx_t n, idx_t k.
 * The routine uses a multiplicative algorithm with reductions
 * via gcd to keep intermediate values small. */
static inline seq_t _binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    /* Use symmetry to minimise the loop length */
    if (k > n - k) k = n - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* term = n - k + i  (numerator part) */
        seq_t term = (seq_t)(n - k + i);
        seq_t denom = (seq_t)i;

        /* Reduce term with denom first */
        seq_t g = _gcd_seq(term, denom);
        term /= g;
        denom /= g;

        /* Reduce result with remaining denom part */
        g = _gcd_seq(result, denom);
        result /= g;
        denom /= g;          /* denom should now be 1 */

        result = result * term;
    }
    return result;
}

/* Main sequence function: a(n) */
seq_t a(idx_t n)
{
    idx_t lower = (n + 2) / 4;   /* floor((n+2)/4) */
    idx_t upper = n / 2;         /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t k = lower; k <= upper; ++k) {
        sum += _binomial(n - k, k);
    }
    return sum;
}
