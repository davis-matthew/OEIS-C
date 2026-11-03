/*
 * A018845  Number of iterations required for the sum of n and its prime
 *          divisors = t to reach a prime (where t replaces n in each
 *          iteration) in A016837.
 * https://oeis.org/A018845
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 764, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9758 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A018845:  number of iterations required for the
    sum of n and its prime divisors to reach a prime.
    Starting from n (>=2), repeat: t = n + sum of prime
    divisors of n (each counted with multiplicity).
    Count the number of such transformations until t is prime. */

#include <stdbool.h>

/* Helper: sum of prime factors of n (counted with multiplicity). */
static seq_t _sum_prime_factors(seq_t n)
{
    seq_t temp = n;
    seq_t sum = 0;
    seq_t p = 2;
    while (p * p <= temp) {
        while (temp % p == 0) {
            sum += p;
            temp /= p;
        }
        if (p == 2) p = 3;          /* skip even numbers after 2 */
        else p += 2;
    }
    if (temp > 1) sum += temp;     /* remaining prime factor */
    return sum;
}

/* Helper: test whether a number is prime (trial division). */
static bool _is_prime(seq_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if (x % 2 == 0) return false;
    seq_t p = 3;
    while (p * p <= x) {
        if (x % p == 0) return false;
        p += 2;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* offset handling: values below 2 are undefined */

    seq_t current = (seq_t)n;
    seq_t count   = 0;

    for (;;) {
        seq_t add = _sum_prime_factors(current);
        current = current + add;
        count++;

        if (_is_prime(current))
            break;
    }

    return count;
}
