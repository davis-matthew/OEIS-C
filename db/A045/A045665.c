/*
 * A045665  Number of 2n-bead balanced binary strings of fundamental period
 *          2n, rotationally equivalent to reverse, complement and reversed
 *          complement.
 * https://oeis.org/A045665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{d|n, d odd} μ(d) * 2^ceil(n/(2*d)) for n>0 * a(0)
 *            = 1 * where μ(d) is the Möbius function
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 52, u64 n <= 114, u128 n <= 240,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>           /* for uint64_t */
#include <stdbool.h>          /* for bool, true, false */
#include "../utils/pow2.h"    /* pow2(idx_t) -> seq_t */

/* compute Möbius function μ(d) for d ≥ 1 (d is assumed odd in our usage) */
static int mobius_uint64(uint64_t d)
{
    int mu = 1;          /* μ(1)=1 */
    uint64_t n = d;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) {
                n /= p;
                ++cnt;
                if (cnt > 1)            /* squared prime factor -> μ=0 */
                    return 0;
            }
            mu = -mu;          /* flip sign for each distinct prime factor */
        }
    }
    if (n > 1)                /* remaining prime factor */
        mu = -mu;
    return mu;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* enumerate odd divisors of n */
    for (idx_t d = 1; d <= n; d += 2) {
        if (n % d != 0) continue;

        int mu = mobius_uint64(d);
        if (mu == 0) continue;

        /* exponent: ceil(n / (2*d)) */
        idx_t exp = (n + 2 * d - 1) / (2 * d);
        seq_t term = pow2(exp);               /* 2^exp */

        if (mu > 0)
            sum_pos += term;
        else
            sum_neg += term;
    }

    /* final sum Σ μ(d) * 2^ceil(...)  (positive) */
    seq_t sum;
    if (sum_pos >= sum_neg)
        sum = sum_pos - sum_neg;
    else
        sum = sum_neg - sum_pos;    /* logically this case does not happen */

    return ((seq_t)n) * sum;
}
