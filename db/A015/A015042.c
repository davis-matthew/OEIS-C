/*
 * A015042  q-Catalan numbers (binomial version) for q=11.
 * https://oeis.org/A015042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = {2 n choose n}_q / (n+1)_q, where q = 11
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (exact integer division, result fits seq_t for all n) */

#include "../utils/big_pow.h"

/* Euclidean GCD for seq_t (works for built‑in and arbitrary precision) */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    const seq_t q = 11;          /* fixed q for this sequence */

    if (n == 0)                 /* a(0) = 1 */
        return 1;

    /* (n+1)_q = (q^(n+1) – 1) / (q – 1) */
    seq_t pow_q_n_plus1 = big_pow(q, n + 1);
    seq_t denom = (pow_q_n_plus1 - 1) / (q - 1);

    /* Compute { 2n choose n }_q by multiplicative cancellation */
    seq_t pow_i = q;                     /* q^i  */
    seq_t pow_n_plus_i = pow_q_n_plus1;  /* q^(n+i)  */
    seq_t result = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t numerator   = pow_n_plus_i - 1; /* q^(n+i) – 1 */
        seq_t denominator = pow_i       - 1; /* q^i       – 1 */

        /* Reduce numerator/denominator by their GCD first */
        seq_t g = seq_gcd(numerator, denominator);
        if (g != 1) {
            numerator   /= g;
            denominator /= g;
        }

        /* Cancel any part of denominator that divides current result */
        g = seq_gcd(result, denominator);
        if (g != 1) {
            result      /= g;
            denominator /= g;
        }

        /* Multiply and then divide by any remaining denominator (should be 1) */
        result *= numerator;
        if (denominator != 1)
            result /= denominator;

        /* Prepare powers for next iteration */
        pow_i          *= q;
        pow_n_plus_i   *= q;
    }

    /* Final division by (n+1)_q (exact) */
    seq_t g = seq_gcd(result, denom);
    if (g != 1) {
        result /= g;
        denom  /= g;
    }
    if (denom != 1)
        result /= denom;

    return result;
}
