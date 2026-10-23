/*
 * A129283  a(n) = (arithmetic derivative of n) + n.
 * https://oeis.org/A129283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + arithmetic_derivative(n) (A003415)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: arithmetic derivative of a positive integer n */
static seq_t arithmetic_derivative(idx_t n)
{
    seq_t N = (seq_t)n;          /* cast once */
    seq_t d = 0;
    idx_t tmp = n;

    /* factor 2 */
    if ((tmp & 1) == 0) {
        int e = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;          /* tmp /= 2 */
            e++;
        }
        d += (seq_t)e * (N / ((seq_t)2));
    }

    /* odd factors */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            d += (seq_t)e * (N / ((seq_t)p));
        }
    }

    /* remaining prime factor > 1 */
    if (tmp > 1) {
        d += N / ((seq_t)tmp);   /* exponent is 1 */
    }

    return d;
}

/* OEIS sequence A129283 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    /* a(n) = n + arithmetic_derivative(n) */
    return (seq_t)n + arithmetic_derivative(n);
}
