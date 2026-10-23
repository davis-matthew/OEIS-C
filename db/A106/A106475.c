/*
 * A106475  An alternating sum of greatest common divisors.
 * https://oeis.org/A106475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum_{k=0..2*n} gcd(2*n-k+1, k+1)*(-1)^k
 * Range:     i8 n <= 34, i16 n <= 1438, i32 n <= 9967, i64 n <= 9967,
 *            i128 n <= 9967, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean GCD for idx_t indices. */
static inline idx_t my_gcd(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Computes a(n) for OEIS A106475. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= 2*n; ++k) {
        idx_t a_val = 2*n - k + 1;   /* 2*n - k + 1 */
        idx_t b_val = k + 1;         /* k + 1 */
        seq_t g = (seq_t)my_gcd(a_val, b_val);
        if (k & 1) {                 /* odd k -> negative sign */
            total -= g;
        } else {                     /* even k -> positive sign */
            total += g;
        }
    }
    return total;
}
