/*
 * A226038  Numbers k such that there are no primes p which divide k+1 and p-1
 *          does not divide k.
 * https://oeis.org/A226038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 6710, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9837 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A226038
   Definition: a(n) is the nth integer k (starting with k=0 for n=1) such that
   for every prime p dividing (k+1), the difference p-1 divides k.
   This implementation evaluates terms directly using trial division and
   stops when the requested index n is reached. */

#include <stdint.h>
#include <stdbool.h>

/* Check the weak Clausen condition for a given k.
   For each prime divisor p of k+1, verify that (k % (p-1)) == 0. */
static bool weak_condition(idx_t k)
{
    idx_t temp = k + 1;         /* number to factorize */

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            if (k % (p - 1) != 0)
                return false;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1) { /* remaining prime factor > sqrt(original temp) */
        if (k % (temp - 1) != 0)
            return false;
    }
    return true;
}

/* Return the nth term (n ≥ 1).  For n == 0 the function returns 0
   as a safe default, although the OEIS offset starts at 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* safety, but not part of the offset */

    idx_t count = 0;          /* number of terms found so far */
    idx_t k = 0;              /* candidate value */

    while (count < n) {
        if (weak_condition(k))
            ++count;          /* one more term satisfies the condition */
        ++k;                  /* move to next candidate */
    }

    return (seq_t)(k - 1);    /* last accepted k is the desired nth term */
}
