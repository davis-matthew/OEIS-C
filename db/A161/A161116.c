/*
 * A161116  a(n) is the number of nontrivial positive divisors of 2n+3.
 * https://oeis.org/A161116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of nontrivial positive divisors of 2n+3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the number of nontrivial positive divisors of 2n+3.
   For n = 0 it returns 0, as required. */
seq_t a(idx_t n)
{
    /* 2n+3 fits into uint64_t for all test cases (n up to 10^4). */
    uint64_t val = (uint64_t)n * 2ULL + 3ULL;
    uint64_t temp = val;
    uint64_t dcnt = 1;                /* multiplicative divisor count */

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            dcnt *= (uint64_t)(exp + 1);
        }
    }
    if (temp > 1) {
        /* remaining prime factor */
        dcnt *= 2ULL;
    }

    /* subtract the two trivial divisors (1 and the number itself) */
    seq_t result = (seq_t)(dcnt - 2ULL);
    return result;
}
