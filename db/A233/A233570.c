/*
 * A233570  Replace the smallest prime factor p in n (if any) with the prime
 *          following p.
 * https://oeis.org/A233570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n / spf(n)) * next_prime(spf(n)) where spf(n) is the
 *            smallest prime factor of n (for n>1), and next_prime(p) is the
 *            first prime greater than p. For n = 1, a(1) = 1
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses only integer arithmetic and the standard C headers. */

#include <stdint.h>
#include <stdbool.h>

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                         return false;
    if (n == 2 || n == 3)              return true;
    if (n % 2 == 0)                    return false;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0)                return false;
    return true;
}

static uint64_t next_prime_uint64(uint64_t p)
{
    uint64_t q = p + 1;
    /* handle the case p == 2 specially to start at 3 */
    if (q == 2) q = 3;
    while (!is_prime_uint64(q))
        ++q;
    return q;
}

static uint64_t smallest_prime_factor_uint64(uint64_t n)
{
    if (n % 2 == 0)                    return 2;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0)                return d;
    return n; /* n is prime */
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t nn   = (uint64_t)n;               /* promote index to 64‑bit */
    uint64_t spf  = smallest_prime_factor_uint64(nn);
    uint64_t next = next_prime_uint64(spf);

    seq_t part = (seq_t)(nn / spf);
    seq_t res  = part * (seq_t)next;
    return res;
}
