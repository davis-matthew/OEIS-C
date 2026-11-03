/*
 * A162195  Sum of proper divisors of n-th nonprime number A018252(n).
 * https://oeis.org/A162195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of proper divisors of the n‑th nonprime integer
 *            (A018252(n)). The function scans integers, filters 1 and
 *            composites, and sums their proper divisors by trial
 *            factorisation
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9958 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Returns true if m is composite or 1, false if m is prime (m >= 2). */
static bool is_nonprime(uint64_t m)
{
    if (m == 1) return true;
    if (m % 2 == 0) return (m != 2);
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) return true;
    }
    return false;
}

/* Sum of proper divisors of m (m >= 1).  Uses trial factorisation. */
static seq_t sum_of_proper_divisors(uint64_t m)
{
    if (m <= 1) return 0;          /* 1 has no proper divisors */

    uint64_t rem = m;
    seq_t sigma = 1;               /* σ(m) */
    for (uint64_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            uint64_t exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            /* term = 1 + p + p^2 + ... + p^exp */
            seq_t term = 1;
            seq_t power = 1;
            for (uint64_t j = 0; j < exp; ++j) {
                power *= (seq_t)p;
                term += power;
            }
            sigma *= term;
        }
    }
    if (rem > 1) {                /* rem is a prime factor > sqrt(m) */
        sigma *= (1 + (seq_t)rem);
    }
    return sigma - (seq_t)m;       /* remove m itself to get proper divisors */
}

/* Return the a(n) value: n ≥ 1.  For n=0 the behaviour is irrelevant. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – undefined behaviour */

    uint64_t target = n;           /* 1‑based index of the nonprime to find */
    uint64_t count = 0;
    uint64_t x = 1;
    seq_t result = 0;

    while (true) {
        if (is_nonprime(x)) {
            ++count;
            if (count == target) {
                result = sum_of_proper_divisors(x);
                break;
            }
        }
        ++x;
    }

    return result;
}
