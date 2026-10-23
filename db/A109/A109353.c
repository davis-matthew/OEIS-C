/*
 * A109353  a(n) is the sum of the distinct prime divisors of A024619(n).
 * https://oeis.org/A109353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the distinct prime divisors of the nth
 *            non‑prime‑power (A024619(n) is the sequence of positive integers
 *            that are not prime powers)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   is_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_power.h"

/* Return the sum of the distinct prime divisors of a(n).  */
seq_t a(idx_t n)
{
    /* Find the nth non‑prime‑power. */
    idx_t count = 0;          /* how many non‑prime‑powers encountered */
    uint64_t candidate = 2;   /* numbers start from 2 */
    uint64_t target = 0;      /* the nth non‑prime‑power found */

    while (count < n)
    {
        if (!is_prime_power(candidate))
        {
            ++count;
            if (count == n) { target = candidate; break; }
        }
        ++candidate;
    }

    /* Factorize target and sum distinct primes. */
    seq_t sum = 0;
    uint64_t tmp = target;
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            sum += (seq_t)p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) sum += (seq_t)tmp;   /* remaining prime factor */

    return sum;
}
