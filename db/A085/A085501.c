/*
 * A085501  Number of prime powers p^k <= n that are not prime (k = 0 or k >
 *          1).
 * https://oeis.org/A085501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 793, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) counts the integers p^k (p prime, k ≥ 2) not exceeding n,
   and also counts 1 (which is p^0), but excludes the prime numbers themselves.
   The implementation keeps a cache of primes up to sqrt(n) and, for each
   prime, counts all its powers up to n. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- integer square root for uint64_t ---------- */
static uint64_t isqrt_u64(uint64_t x)
{
    if (x <= 1) return x;
    uint64_t lo = 1, hi = x / 2, ans = 1;
    while (lo <= hi)
    {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t q   = x / mid;
        if (mid <= q)
        {
            ans = mid;
            lo  = mid + 1;
        }
        else
            hi = mid - 1;
    }
    return ans;
}

/* ---------- dynamic prime sieve cache ---------- */
static uint64_t *prime_list   = NULL;   /* primes in ascending order   */
static size_t    prime_count  = 0;      /* number of primes stored     */
static uint64_t  prime_limit  = 0;      /* highest value already sieved */

static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_limit) return; /* nothing new to add */

    uint64_t start = (prime_limit == 0 ? 2 : prime_limit + 1);
    for (uint64_t cand = start; cand <= limit; ++cand)
    {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i)
        {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime)
        {
            size_t new_sz = prime_count + 1;
            uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                new_sz * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            prime_list = tmp;
            prime_list[prime_count++] = cand;
        }
    }
    prime_limit = limit;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    /* The OEIS sequence is defined for n >= 1.  For safety we return 0
       when n == 0, but callers should not request it. */
    if (n == 0) return 0;

    seq_t count = 1; /* count the value 1 (p^0) */

    seq_t n_seq = (seq_t)n;           /* promote to seq_t for arithmetic  */
    uint64_t maxp = isqrt_u64((uint64_t)n);

    /* Ensure we have all primes up to sqrt(n). */
    ensure_primes_upto(maxp);

    /* For each prime, count its powers p^k (k >= 2) not exceeding n. */
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t p = prime_list[i];
        if (p > maxp) break;       /* no further primes needed */

        seq_t power = (seq_t)p * (seq_t)p;   /* start with p^2 */

        while (power <= n_seq)
        {
            ++count;                              /* found an allowed power */

            /* Avoid overflow before the next multiplication. */
            if (power > n_seq / (seq_t)p) break;
            power *= (seq_t)p;
        }
    }

    return count;
}
