/*
 * A055309  Number of labeled rooted trees with n nodes and 8 leaves.
 * https://oeis.org/A055309
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! *
 *            (n-8)^2*(n-7)^2*(n-6)*(n-5)*(n-4)*(n-3)*(n-2)*(n-1)*(9*n^4 -
 *            234*n^3 + 2211*n^2 - 8938*n + 12960)/234101145600
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 12, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence starts at n = 9 */

#include <stdint.h>

static inline void mul_factor(seq_t *res, seq_t f, int *rem2, int *rem3, int *rem5, int *rem7)
{
    /* cancel factors of f with remaining denominator primes */
    while (*rem2 && (f % 2 == 0)) { f /= 2; (*rem2)--; }
    while (*rem3 && (f % 3 == 0)) { f /= 3; (*rem3)--; }
    while (*rem5 && (f % 5 == 0)) { f /= 5; (*rem5)--; }
    while (*rem7 && (f % 7 == 0)) { f /= 7; (*rem7)--; }

    /* cancel factors from current result to keep it small */
    while (*rem2 && (*res % 2 == 0)) { *res /= 2; (*rem2)--; }
    while (*rem3 && (*res % 3 == 0)) { *res /= 3; (*rem3)--; }
    while (*rem5 && (*res % 5 == 0)) { *res /= 5; (*rem5)--; }
    while (*rem7 && (*res % 7 == 0)) { *res /= 7; (*rem7)--; }

    /* multiply by the factor */
    *res *= f;

    /* cancel any remaining denominator primes from the new result */
    while (*rem2 && (*res % 2 == 0)) { *res /= 2; (*rem2)--; }
    while (*rem3 && (*res % 3 == 0)) { *res /= 3; (*rem3)--; }
    while (*rem5 && (*res % 5 == 0)) { *res /= 5; (*rem5)--; }
    while (*rem7 && (*res % 7 == 0)) { *res /= 7; (*rem7)--; }
}

seq_t a(idx_t n)
{
    if (n < 9) return (seq_t)0;   /* undefined for n < 9, value irrelevant */

    seq_t result = (seq_t)1;
    int rem2 = 18, rem3 = 6, rem5 = 2, rem7 = 2;   /* prime exponents in denominator */

    /* factorial n! */
    for (idx_t i = 2; i <= n; ++i)
        mul_factor(&result, (seq_t)i, &rem2, &rem3, &rem5, &rem7);

    /* (n-8)^2 and (n-7)^2 */
    seq_t x = (seq_t)n;
    for (int k = 0; k < 2; ++k) mul_factor(&result, x - 8, &rem2, &rem3, &rem5, &rem7);
    for (int k = 0; k < 2; ++k) mul_factor(&result, x - 7, &rem2, &rem3, &rem5, &rem7);

    /* (n-6) to (n-1) */
    mul_factor(&result, x - 6, &rem2, &rem3, &rem5, &rem7);
    mul_factor(&result, x - 5, &rem2, &rem3, &rem5, &rem7);
    mul_factor(&result, x - 4, &rem2, &rem3, &rem5, &rem7);
    mul_factor(&result, x - 3, &rem2, &rem3, &rem5, &rem7);
    mul_factor(&result, x - 2, &rem2, &rem3, &rem5, &rem7);
    mul_factor(&result, x - 1, &rem2, &rem3, &rem5, &rem7);

    /* polynomial part */
    seq_t n2 = x * x;
    seq_t n3 = n2 * x;
    seq_t n4 = n3 * x;
    seq_t poly = 9 * n4 - 234 * n3 + 2211 * n2 - 8938 * x + 12960;

    /* cancel the remaining prime factors from the polynomial */
    while (rem2 && (poly % 2 == 0)) { poly /= 2; rem2--; }
    while (rem3 && (poly % 3 == 0)) { poly /= 3; rem3--; }
    while (rem5 && (poly % 5 == 0)) { poly /= 5; rem5--; }
    while (rem7 && (poly % 7 == 0)) { poly /= 7; rem7--; }

    /* multiply the polynomial */
    result *= poly;

    /* Any residual denominator factors must now divide the result */
    while (rem2) { result /= 2; rem2--; }
    while (rem3) { result /= 3; rem3--; }
    while (rem5) { result /= 5; rem5--; }
    while (rem7) { result /= 7; rem7--; }

    return result;
}
