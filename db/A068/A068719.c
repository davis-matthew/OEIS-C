/*
 * A068719  Arithmetic derivative of even numbers: a(n) = n+2*A003415(n).
 * https://oeis.org/A068719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 2*A003415(n) where A003415(n) is the arithmetic
 *            derivative of n: if n = ∏ p_i^e_i then n' = n * Σ e_i/p_i
 * Range:     u8 n <= 47, u16 n <= 6143, u32 n <= 9762, u64 n <= 9762,
 *            u128 n <= 9762, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: arithmetic derivative of an integer n (n >= 1) */
static seq_t arithmetic_derivative(idx_t n)
{
    if (n <= 1) return 0;

    seq_t sum = 0;
    idx_t tmp = n;

    for (idx_t p = 2; (idx_t)p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint8_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            /* (n / p) fits in seq_t; e is small */
            sum += ((seq_t)(n / p)) * (seq_t)e;
        }
    }

    if (tmp > 1) {          /* tmp is a remaining prime factor */
        sum += ((seq_t)(n / tmp)) * (seq_t)1;
    }

    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1; term 0 is unused but handled safely */
    seq_t der = arithmetic_derivative(n);
    return (seq_t)n + (seq_t)2 * der;
}
