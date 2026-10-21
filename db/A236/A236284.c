/*
 * A236284  a(n) = tau(n)^n, where tau(n) = A000005(n) = the number of
 *          divisors of n.
 * https://oeis.org/A236284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n)^n where tau(n) = number of divisors of n (A000005)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the n-th term of OEIS sequence A236284.  The function
 * accepts any n >= 1 and is independent of call order.
 * It works for all supported seq_t sizes, including arbitrary‑precision
 * implementations, by performing the exponentiation with the native
 * seq_t type. */
seq_t a(idx_t n)
{
    /* Guard against a call with n == 0; the sequence is defined for n>=1. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* --- Compute tau(n) = number of divisors of n --- */
    uint64_t tmp = (uint64_t)n;
    uint64_t tau = 1;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }
    if (tmp > 1) {          /* Remaining prime factor */
        tau *= 2;
    }

    /* --- Compute tau^n using the native seq_t type --- */
    seq_t result = (seq_t)1;
    seq_t base    = (seq_t)tau;

    for (idx_t i = 0; i < n; ++i) {
        result *= base;
    }

    return result;
}
