/*
 * A056224  Image of number of divisors sequence (A000005) under "little
 *          Hankel" transform that sends [c_0, c_1, ...] to [d_0, d_1, ...]
 *          where d_n = c_n^2 - c_{n+1}*c_{n-1}.
 * https://oeis.org/A056224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = τ(n+1)^2 – τ(n)·τ(n+2) (τ(k) = number of divisors of k)
 * Range:     i8 n <= 58, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ---------- helper: divisor count ------------------------------------- */
static seq_t divisor_count(idx_t n)
{
    /* τ(1) = 1, τ(k) = 1 for k = 0 is never used but handled safely */
    if (n <= 1)
        return 1;

    seq_t result = 1;
    idx_t t = n;

    /* trial division to factor t */
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            idx_t exp = 0;
            while (t % p == 0) {
                t /= p;
                ++exp;
            }
            /* each prime power contributes (exp+1) to τ(t) */
            result *= (exp + 1);
        }
    }
    /* any remaining prime factor > sqrt(t) contributes multiplicatively 
       with exponent 1, i.e. factor 2 */
    if (t > 1)
        result *= 2;

    return result;
}

/* ---------- main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    seq_t tau_n   = divisor_count(n);     /* τ(n)   */
    seq_t tau_n1  = divisor_count(n + 1); /* τ(n+1) */
    seq_t tau_n2  = divisor_count(n + 2); /* τ(n+2) */

    /* a(n) = τ(n+1)^2 – τ(n)·τ(n+2) */
    return tau_n1 * tau_n1 - tau_n * tau_n2;
}
