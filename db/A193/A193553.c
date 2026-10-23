/*
 * A193553  Sum of divisors of 4*n.
 * https://oeis.org/A193553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 4409, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A193553: a(n) = sigma(4*n)  (Sum of divisors of 4*n) */
#include <stdint.h>
#include <stddef.h>

/* Computes sigma(m) for an odd integer m >= 1.
   Uses a simple O(sqrt(m)) divisor enumeration, returning the sum
   as a seq_t value. */
static seq_t sigma_odd(idx_t m)
{
    if (m == 1) {
        return (seq_t)1;
    }
    seq_t sum = (seq_t)1;           /* divisor 1 */
    for (idx_t i = 3; i * i <= m; i += 2) {
        if (m % i == 0) {
            sum += (seq_t)i;
            idx_t d2 = m / i;
            if (d2 != i) {
                sum += (seq_t)d2;
            }
        }
    }
    sum += (seq_t)m;                /* divisor m itself */
    return sum;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;           /* offset is 1, ignore n=0 */
    }

    /* Separate the exponent of 2 in n and its odd part. */
    idx_t tmp = n;
    idx_t e = 0;                   /* exponent of 2 in n */
    while ((tmp & 1) == 0) {
        ++e;
        tmp >>= 1;
    }
    idx_t odd_part = tmp;          /* m in the discussion */

    /* sigma(4*n) = sigma(2^(e+2)) * sigma(odd_part)  */
    seq_t sigma_m = sigma_odd(odd_part);

    /* Compute sigma(2^(e+2)) = 2^(e+3) - 1. */
    idx_t exp = e + 3;             /* e+3 = (e+2)+1 */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        pow2 <<= 1;                /* multiply by 2 each iteration */
    }
    seq_t sigma_two = pow2 - (seq_t)1;

    return sigma_m * sigma_two;
}
