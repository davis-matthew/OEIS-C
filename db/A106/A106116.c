/*
 * A106116  Primes without {0, 1} as digits.
 * https://oeis.org/A106116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   this function searches progressively larger integers, keeps only
 *            those that are prime and do not contain the digits 0 or 1, and
 *            returns the n-th such prime. The sequence starts at n = 1
 * Range:     u8 n <= 22, u16 n <= 2137, u32 n <= 50000, u64 n <= 50000,
 *            u128 n <= 50000, big any n
 * Validated: standard, pass, 73 terms through n = 7965 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

/* Helper: test that all decimal digits are in {2,3,4,5,6,7,8,9}. */
static bool good_digit(uint64_t x)
{
    if (x == 0) return false;          /* not a prime candidate */
    while (x > 0) {
        uint64_t d = x % 10;
        if (d == 0 || d == 1)
            return false;
        x /= 10;
    }
    return true;
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset starts at 1; behaviour below 1 is undefined */
        return (seq_t)0;

    idx_t count = 0;
    uint64_t cur = 2;

    while (true) {
        if (is_prime_u64(cur) && good_digit(cur)) {
            ++count;
            if ((idx_t)count == n)
                return (seq_t)cur;
        }
        if (cur == 2)
            cur = 3;
        else
            cur += 2;
    }
}
