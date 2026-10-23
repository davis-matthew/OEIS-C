/*
 * A193349  Sum of odd divisors of tau(n).
 * https://oeis.org/A193349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of odd divisors of tau(n) (tau(n) = number of
 *            divisors of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_smallest_prime_factor.h"   /* provides uint64_t _smallest_prime_factor(uint64_t n) */

/* Helper: multiply base^exp (exp ≥ 0) using seq_t arithmetic. */
static seq_t pow_seq(seq_t base, int exp)
{
    seq_t result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

/* Helper: sum of odd divisors of a 64‑bit integer m. */
static seq_t sum_odd_divisors_of_uint64(uint64_t m)
{
    /* remove all factors of 2 – they never appear in odd divisors */
    while ((m & 1) == 0) {
        m >>= 1;
    }

    seq_t sum = 1;
    while (m > 1) {
        uint64_t p = _smallest_prime_factor(m);
        int cnt = 0;
        while (m % p == 0) {
            m /= p;
            ++cnt;
        }
        seq_t p_seq = (seq_t)p;
        seq_t term = (pow_seq(p_seq, cnt + 1) - 1) / (p_seq - 1);
        sum *= term;
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n=0; safe return for robustness */

    uint64_t t = (uint64_t)n;
    seq_t tau = 1;         /* tau(n) = product over (exponent+1) */

    while (t > 1) {
        uint64_t p = _smallest_prime_factor(t);
        int exp = 0;
        while (t % p == 0) {
            t /= p;
            ++exp;
        }
        tau *= (seq_t)(exp + 1);
    }

    /* tau fits into uint64_t for 64‑bit n; no overflow in seq_t for all builds */
    uint64_t tau_val = (uint64_t)tau;
    return sum_odd_divisors_of_uint64(tau_val);
}
