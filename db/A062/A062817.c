/*
 * A062817  a(n) = Sum_{i=0..n} i^(n - i)*(n - i)^i.
 * https://oeis.org/A062817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n} i^(n-i)*(n-i)^i (OEIS A062817, offset 1) The
 *            function is implemented directly using the definition. It uses
 *            fast exponentiation (binary exponentiation) and relies only on
 *            the operations allowed on seq_t (addition, multiplication). No
 *            external helper functions are required
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t pow_seq_t_custom(seq_t base, idx_t exp)
{
    seq_t result = 1;             /* seq_t one */
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* values below offset are irrelevant */

    seq_t sum = 0;
    idx_t half = n / 2;           /* floor(n/2) */

    for (idx_t i = 1; i <= half; ++i) {
        seq_t pow1 = pow_seq_t_custom((seq_t)i, (idx_t)(n - i));
        seq_t pow2 = pow_seq_t_custom((seq_t)(n - i), (idx_t)i);
        seq_t term = pow1 * pow2;

        /* term(i) equals term(n-i). Add appropriately. */
        if (i == n - i)  /* only when n is even and i is the middle term */
            sum += term;
        else
            sum += term + term;   /* add twice */
    }

    return sum;
}
