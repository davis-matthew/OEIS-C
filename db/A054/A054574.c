/*
 * A054574  Begin with n-th prime, add its prime divisors (itself), repeat
 *          until reach a new prime; sequence gives prime reached.
 * https://oeis.org/A054574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = starting from the n‑th prime p, repeatedly add the sum of
 *            p’s prime divisors (with multiplicity) until a new prime is
 *            reached; that prime is a(n). a(1) = 23, a(2) = 11, …
 * Range:     u8 n <= 12, u16 n <= 650, u32 n <= 3499, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/get_prime.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_prime_big.h"

/* Sum of the prime factors of x (with multiplicity).  Works for values that
   fit into a uint64_t, which is sufficient for the expected range of n. */
static seq_t sum_prime_factors(seq_t x)
{
    uint64_t ux = (uint64_t)x;      /* work in 64‑bit arithmetic        */
    seq_t  sum = 0;
    while (ux > 1) {
        uint64_t spf = _smallest_prime_factor(ux); /* smallest prime factor */
        sum += (seq_t)spf;
        ux /= spf;
    }
    return sum;
}

/* Main sequence routine.  Returns the new prime reached by iterating the
   described process.  The function works for any idx_t and seq_t
   types used by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)                    /* safety for invalid indices      */
        return 0;

    seq_t cur = (seq_t)get_prime(n);      /* n‑th prime, 1‑based           */
    while (1) {
        seq_t add = sum_prime_factors(cur);
        seq_t nxt = cur + add;
        if (is_prime_big(nxt))
            return nxt;
        cur = nxt;
    }
}
