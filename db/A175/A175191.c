/*
 * A175191  a(n) = the smallest positive integer such that (the n-th
 *          prime)+2*a(n) is composite.
 * https://oeis.org/A175191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest positive integer k such that the n-th prime
 *            p_n plus 2·k is composite. The implementation generates primes
 *            up to the required index and tests whether p_n+2 is prime; if it
 *            is prime, k=2 (otherwise k=1). The only exception is p_2 = 3,
 *            for which k=3
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence values are confined to 1, 2 or 3; the implementation
   keeps the obvious edge‑case for the prime 3. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* static storage for the prime sieve and the list of primes */
static bool  *sieve      = NULL;        /* sieve array, index 0..limit-1   */
static uint32_t *primes  = NULL;        /* array of primes up to limit    */
static idx_t    sieve_limit = 0;        /* current sieve size            */
static idx_t    primes_count = 0;       /* number of primes stored       */

/* Estimate an upper bound on the n-th prime using the inequality
   p_n < n(ln n + ln ln n) for n ≥ 6.  For small n we simply use
   a fixed safe value. */
static idx_t estimate_limit(idx_t n)
{
    if (n < 6) return 15; /* 6th prime is 13, so 15 is safe */
    double dn = (double)n;
    double bound = dn * (log(dn) + log(log(dn))) + 10.0; /* margin */
    return (idx_t)bound + 1;
}

/* Re‑sieve up to at least the requested number of primes.  If a
   larger limit is required, the sieve array is reallocated.  The
   list of primes is rebuilt at each re‑sieve.  Subsequent calls
   that request a smaller n reuse the existing data. */
static void ensure_primes(idx_t n)
{
    if (primes_count >= n) return;          /* enough primes already stored */

    idx_t new_limit = estimate_limit(n);

    if (new_limit > sieve_limit) {           /* need a larger sieve */
        bool *new_sieve = (bool*)calloc((size_t)new_limit, sizeof(bool));
        if (!new_sieve) return;              /* allocation failure */
        free(sieve);
        sieve = new_sieve;
        sieve_limit = new_limit;
    }

    /* mark composites */
    if (sieve_limit > 0) sieve[0] = sieve[1] = true;
    for (idx_t i = 2; i * i < sieve_limit; ++i)
        if (!sieve[i])
            for (idx_t j = i * i; j < sieve_limit; j += i)
                sieve[j] = true;

    /* collect primes into the primes array */
    free(primes);
    primes = (uint32_t*)malloc((size_t)sieve_limit * sizeof(uint32_t));
    if (!primes) return;
    primes_count = 0;
    for (idx_t i = 2; i < sieve_limit; ++i)
        if (!sieve[i]) primes[primes_count++] = (uint32_t)i;
}

/* Test whether x is prime using the pre‑computed prime list.  The
   function assumes that sieve_limit > sqrt(x). */
static bool is_prime_unsigned(idx_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < primes_count; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > (uint64_t)x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* --- required function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of bounds, undefined behaviour */

    /* ensure we have the first n primes available */
    ensure_primes(n);

    uint32_t p = primes[n - 1];    /* p_n, 1‑based indexing */

    if (p == 3u) /* special case: 3 + 2*3 = 9 composite, 3 + 2*1 and *2 are prime */
        return (seq_t)3;

    if (!is_prime_unsigned((idx_t)p + 2))
        return (seq_t)1;           /* p + 2 is composite → k = 1 */

    /* otherwise p + 2 is prime; for all odd primes > 3, p + 4 is composite,
       so the smallest k is 2. */
    return (seq_t)2;
}
