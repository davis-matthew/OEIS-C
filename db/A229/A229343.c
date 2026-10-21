/*
 * A229343  Moebius function of the arithmetic derivative of n: a(n) = mu(n').
 * https://oeis.org/A229343
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = μ(n') where n' is the arithmetic derivative of n (OEIS
 *            A229343). For n < 2 the value is undefined; we return 0
 * Range:     i8 n <= 3353, i16 n <= 9748, i32 n <= 9748, i64 n <= 9748,
 *            i128 n <= 9748, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper to compute the arithmetic derivative of n (n ≥ 2). */
static seq_t arithmetic_derivative(idx_t n)
{
    uint64_t original = n;        /* keeps the original n for the term n/p   */
    uint64_t sum = 0;             /* derivative fits in uint64_t for our range */

    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            sum += exp * (original / p);
        }
    }
    /* If there's a remaining prime factor > sqrt(original)  */
    if (temp > 1) {               /* temp itself is a prime */
        sum += 1 * (original / temp);
    }
    return (seq_t)sum;
}

/* Computes the Möbius function μ(m) for any non‑negative integer m. */
static seq_t mobius_uint64(uint64_t m)
{
    if (m == 0) return 0;
    if (m == 1) return 1;
    int k = 0;                    /* count of distinct prime factors  */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
                if (exp > 1)   /* square factor → μ = 0 */
                    return 0;
            }
            ++k;
        }
    }
    if (m > 1) {                  /* remaining prime factor  */
        ++k;
    }
    return (seq_t)((k % 2 == 0) ? 1 : -1);
}

/* External correct version of the OEIS function. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* n=0,1 are outside the offset */
    seq_t nprime = arithmetic_derivative(n);
    return mobius_uint64((uint64_t)nprime);
}
