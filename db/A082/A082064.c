/*
 * A082064  Greatest common prime-divisor of phi(n) and sigma(n) = A000203(n);
 *          a(n)=1 if no common prime-divisor exists.
 * https://oeis.org/A082064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest common prime divisor of φ(n) and σ(n), with
 *            a(n)=1 when φ(n) and σ(n) share no common prime divisor
 * Range:     u8 n <= 793, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9635 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t d = 2;
    for (; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / (seq_t)d;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Greatest common divisor for seq_t values. */
static seq_t gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Compute φ(n) and σ(n) */
    seq_t phi_val   = phi(n);
    seq_t sigma_val = sigma_of(n);

    /* Common divisor of φ(n) and σ(n) */
    seq_t g = gcd(phi_val, sigma_val);

    /* If there is no common prime divisor, return 1 */
    if (g <= 1) return (seq_t)1;

    /* Find the greatest prime factor of g */
    uint64_t g64   = (uint64_t)g;
    uint64_t temp  = g64;
    uint64_t largest = 1;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            largest = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) largest = temp;  /* the remaining prime factor */

    return (seq_t)largest;
}
