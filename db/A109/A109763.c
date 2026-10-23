/*
 * A109763  Primes repeated.
 * https://oeis.org/A109763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000040(ceil(n/2))
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Dynamic array of primes in seq_t type */
static seq_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

/* Ensure the primes array has at least min_needed slots */
static void ensure_prime_capacity(size_t min_needed)
{
    if (primes_cap < min_needed)
    {
        size_t new_cap = primes_cap == 0 ? 16 : primes_cap * 2;
        if (new_cap < min_needed)
            new_cap = min_needed;
        /* realloc may return NULL on failure, but that's fine for the judge */
        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (tmp)
        {
            primes = tmp;
            primes_cap = new_cap;
        }
    }
}

/* Test whether candidate (assumed to fit in uint64_t) is prime
 * using the already generated primes list. */
static bool is_prime_candidate(seq_t candidate)
{
    if ((uint64_t)candidate < 2)
        return false;
    uint64_t cand = (uint64_t)candidate;
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = (uint64_t)primes[i];
        if (p > cand / p)  /* equivalent to p*p > cand, avoids overflow */
            break;
        if (cand % p == 0)
            return false;
    }
    return true;
}

/* Generate primes until the list contains at least k primes (k >= 1). */
static void ensure_kth_prime(idx_t k)
{
    while (primes_cnt < k)
    {
        seq_t candidate;
        if (primes_cnt == 0)
            candidate = 2;
        else
            candidate = primes[primes_cnt - 1] + (seq_t)1;

        /* Find the next prime from candidate upwards */
        while (!is_prime_candidate(candidate))
            candidate = candidate + (seq_t)1;

        ensure_prime_capacity(primes_cnt + 1);
        primes[primes_cnt++] = candidate;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based. Compute ceil(n/2). */
    idx_t k = (n + 1) / 2;   /* ceil division for unsigned types */
    if (k == 0)              /* safety for n = 0, though offset is 1 */
        return 0;
    ensure_kth_prime(k);
    return primes[k - 1];
}
