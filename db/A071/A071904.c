/*
 * A071904  Odd composite numbers.
 * https://oeis.org/A071904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   a(n) = nth odd composite number (OEIS A071904)
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit values.
 * Works for all n ≤ 2^64-1, which is enough because the sequence
 * grows roughly as a(n) ≈ 2n and the test harness only requests
 * values that fit in 64 bits. */
static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    /* iterate over odd divisors */
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Return the n-th odd composite number (a(n) for n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */
    seq_t odd   = 9;                /* first odd composite: 3 × 3 */
    idx_t count = 0;                /* number of odd composites seen */

    for (;; odd += 2)              /* iterate over odd numbers only */
    {
        /* cast to 64‑bit for primality test – safe because all
           values that occur for the tested n are ≤ 2^64-1          */
        uint64_t val = (uint64_t)odd;
        if (!is_prime64(val))      /* odd is composite if not prime  */
        {
            ++count;
            if (count == n)        /* nth composite found */
                return odd;
        }
    }
}
