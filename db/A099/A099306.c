/*
 * A099306  n''', the third arithmetic derivative of n.
 * https://oeis.org/A099306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(A003415(A003415(n)))
 * Range:     u8 n <= 31, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute the arithmetic derivative d(x) of a non‑negative integer x.
   The function works for any seq_t value that fits in the build’s
   integer type. */
static seq_t arithmetic_derivative(seq_t val)
{
    if (val <= (seq_t)1)
        return (seq_t)0;

    seq_t result = (seq_t)0;
    seq_t temp   = val;
    idx_t  p     = 2;

    /* trial‑division up to sqrt(temp) */
    while ((seq_t)p <= temp / (seq_t)p) {
        if (temp % (seq_t)p == 0) {
            unsigned int exp   = 0;
            seq_t     factor   = val / (seq_t)p; /* constant for this prime */

            while (temp % (seq_t)p == 0) {
                temp /= (seq_t)p;
                exp++;
            }
            result += factor * exp;
        }
        if (p == 2)
            p = 3;
        else
            p += 2;
    }

    /* remaining prime factor, if any */
    if (temp > (seq_t)1)
        result += val / temp;

    return result;
}

/* Public function requested by the framework. */
seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;
    seq_t d1 = arithmetic_derivative(x);
    seq_t d2 = arithmetic_derivative(d1);
    seq_t d3 = arithmetic_derivative(d2);
    return d3;
}
