/*
 * A065769  Prime cascade: multiplicative with a(prime(m)^k) = prime(m-1) *
 *          prime(m)^(k-1).
 * https://oeis.org/A065769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = ∏_{p^e‖n} (prev_prime(p) * p^{e-1}) (a(1)=1, a(2)=1)
 * Range:     u8 n <= 262, u16 n <= 9784, u32 n <= 9784, u64 n <= 9784,
 *            u128 n <= 9784, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* simple primality test for n ≤ 10000 (used only for previous‑prime search) */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2ULL == 0)  return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* previous prime: returns 1 if p <= 2, else the largest prime < p */
static uint64_t prev_prime(uint64_t p)
{
    if (p <= 2) return 1;          /* by definition of the sequence */
    for (uint64_t x = p - 1; x > 1; --x)
        if (is_prime_u64(x)) return x;
    return 1;                      /* never reached for p ≥ 2 */
}

/* exponentiation in seq_t (assumes seq_t is an unsigned integer type) */
static seq_t pow_seq_t(uint64_t base, uint64_t exp)
{
    seq_t result = 1;
    seq_t b      = (seq_t)base;
    while (exp) {
        if (exp & 1ULL) result *= b;
        b *= b;
        exp >>= 1ULL;
    }
    return result;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1)=1, a(0) ignored */

    uint64_t m      = (uint64_t)n;
    seq_t   result  = 1;

    /* factorization */
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            uint64_t e = 0;
            while (m % d == 0) { m /= d; ++e; }

            uint64_t prev = prev_prime(d);
            seq_t term = (seq_t)prev;
            if (e > 1) term *= pow_seq_t(d, e - 1);

            result *= term;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1) {
        uint64_t prev = prev_prime(m);          /* exponent e = 1 */
        result *= (seq_t)prev;
    }

    return result;
}
