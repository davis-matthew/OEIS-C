/*
 * A083390  Integers m such that 2m + 1 divides lcm(1,3,5,...,2m - 1).
 * https://oeis.org/A083390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Validated: standard, pass, 80 terms through n = 2301 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A083390:
 * Integers m such that 2m+1 divides lcm(1,3,5,…,2m-1)
 *
 * The routine generates m in increasing order, testing the
 * condition by factoring (2m+1) and verifying that each prime
 * power dividing it does not exceed 2m-1 (otherwise that power
 * cannot be present in the lcm).  Only composite odd numbers
 * can satisfy the condition.
 */
#include <stdbool.h>

/* Test whether m satisfies the lcm divisibility condition.
 * Returns true iff 2m+1 divides lcm(1,3,5,…,2m-1). */
static bool m_is_good(idx_t m)
{
    seq_t p = 2 * m + 1;          /* odd number to test */
    seq_t limit = 2 * m - 1;      /* maximum exponent needed */

    /* 2m+1 must be composite; primes can never satisfy the test */
    if (p <= 3) return false;

    seq_t temp = p;
    bool composite = false;

    /* Factor p using trial division by odd integers.
     * For each prime factor i, accumulate the prime power.
     */
    for (seq_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            composite = true;
            seq_t power = 1;
            while (temp % i == 0) {
                temp /= i;
                if (power > limit / i) return false; /* prime power too large */
                power *= i;
            }
        }
    }

    /* Remaining factor > 1 is a prime.
     * It must be ≤ limit to keep the corresponding power at most 1. */
    if (temp > 1) {
        composite = true;
        if (temp > limit) return false;
    }

    return composite;
}

/* External entry point as required by the exercise. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 1;

    while (1) {
        if (m_is_good(m)) {
            count++;
            if (count == n) return (seq_t) m;
        }
        m++;
    }
}
