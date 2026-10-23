/*
 * A033885  a(n) = 3*n - sum of divisors of n.
 * https://oeis.org/A033885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*n - σ(n) (σ = sum of divisors)
 * Range:     i8 n <= 66, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A033885. */

#include <stdint.h>

/* ---------- helper for σ(n) ---------- */
static seq_t sigma(idx_t n)
{
    if (n <= 1) {
        /* σ(1) = 1, σ(0) is unused but harmless */
        return (seq_t)1;
    }

    seq_t sum = (seq_t)1;          /* start with divisor 1 */

    /* iterate i from 2 up to floor(sqrt(n)) */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            sum += (seq_t)i;                 /* divisor i */
            idx_t j = n / i;
            if (j != i) {
                sum += (seq_t)j;             /* paired divisor n/i */
            }
        }
    }

    /* add n itself as a divisor when n > 1 */
    sum += (seq_t)n;
    return sum;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    /* n is assumed to be >= 1.  The function may be called in any order. */
    seq_t sigma_n = sigma(n);
    return (seq_t)3 * (seq_t)n - sigma_n;
}
