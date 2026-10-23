/*
 * A193421  E.g.f.: Sum_{n>=0} x^n * exp(n^2*x).
 * https://oeis.org/A193421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implementation of OEIS sequence A193421
 * Formula: a(n) = n! * Σ_{k=0..n} (n-k)^(2k) / k!
 *
 * The function uses only integer arithmetic and a small helper
 * for integer exponentiation.  It works for any seq_t type supported
 * by the build environment (uint8_t … unsigned __int128 or an
 * arbitrary‑precision integer class).
 */
#include <stddef.h>   /* for size_t, idx_t typedefs are supplied by the build */

/* Helper: integer power using repeated squaring */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != 0) {
        if (exp & 1)
            result *= base;
        if (exp > 1)
            base *= base;
        exp >>= 1;
    }
    return result;
}

/* Computing a(n) with the explicit summation */
seq_t a(idx_t n)
{
    /* Compute n!  */
    seq_t fact_n = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n *= (seq_t)i;

    seq_t sum = (seq_t)0;
    seq_t pk   = fact_n;           /* pk = n! / k! initially with k = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t base    = (seq_t)(n - k);
        seq_t pow_val = pow_seq(base, 2 * k);   /* (n-k)^(2k) */
        seq_t term    = pk * pow_val;
        sum += term;

        if (k < n)
            pk /= (seq_t)(k + 1);            /* next pk = n! / (k+1)! */
    }
    return sum;
}
