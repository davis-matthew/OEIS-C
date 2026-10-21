/*
 * A079814  Odd integers k such that phi(k)/k < 6/Pi^2 where phi = A000010.
 * https://oeis.org/A079814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 4366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Condition: a(n) is the n‑th odd integer k such that phi(k)/k < 6/pi^2;
   computed by scanning odd numbers and testing the inequality. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Choose a limit large enough so that sqrt(max_candidate) is below it.
   For n up to about 10000 the required numbers are below 1e5, so
   this is more than enough. */
#define MAX_PRIME 100000

static uint32_t primes[MAX_PRIME];
static size_t prime_count = 0;

/* Simple sieve to fill the primes array (once). */
static void init_primes(void)
{
    if (prime_count > 0) return;            /* already done */
    bool sieve[MAX_PRIME + 1];
    for (int i = 0; i <= MAX_PRIME; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= MAX_PRIME; ++i)
        if (sieve[i])
            for (int j = i * i; j <= MAX_PRIME; j += i)
                sieve[j] = false;
    for (int i = 2; i <= MAX_PRIME; ++i)
        if (sieve[i]) primes[prime_count++] = (uint32_t)i;
}

/* Returns true iff the odd integer n satisfies phi(n)/n < 6/pi^2. */
static bool good_odd(uint64_t n)
{
    if ((n & 1) == 0) return false;        /* only odd numbers are needed */

    init_primes();
    uint64_t tmp = n;
    double ratio = 1.0;

    /* Compute φ(n)/n as a double product.  This is safe for the
       values we encounter (~< 1e6) and avoids the need for a
       high‑precision fractional comparison. */
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > tmp) break;
        if (tmp % p == 0)
        {
            ratio *= (1.0 - 1.0 / (double)p);
            do { tmp /= p; } while (tmp % p == 0);
        }
    }
    if (tmp > 1)                      /* remaining prime factor */
        ratio *= (1.0 - 1.0 / (double)tmp);

    /* 6/pi^2 ≈ 0.607927115 */
    const double THRESHOLD = 0.607927115101;
    return ratio < THRESHOLD;
}

/* Sequence a(n).  Works for any n ≥ 1.  Requests are independent. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but return 0 */

    uint64_t count  = 0;
    uint64_t candidate = 3;        /* start with the first odd > 1 */

    while (1)
    {
        if (good_odd(candidate))
        {
            ++count;
            if (count == (uint64_t)n)
                return (seq_t)candidate;
        }
        candidate += 2;            /* only odd numbers */
    }
}
