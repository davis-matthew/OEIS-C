/*
 * A087766  Primes whose reversal is a multiple of 19.
 * https://oeis.org/A087766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 340, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = the nth prime p such that reverse(p) is a multiple of 19.
   Computed by an incremental sieve and checking each prime.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *candidate = NULL;   /* array of primes satisfying the property */
static size_t      candidate_cnt = 0;
static size_t      candidate_cap = 0;
static uint64_t    current_limit = 0;   /* upper bound of primes currently sieved */

/* Append a value to the candidate array, expanding if necessary. */
static void append_candidate(uint64_t value)
{
    if (candidate_cnt == candidate_cap)
    {
        candidate_cap = candidate_cap ? candidate_cap * 2 : 4;
        candidate = (uint64_t *)realloc(candidate, candidate_cap * sizeof(uint64_t));
    }
    candidate[candidate_cnt++] = value;
}

/* Recompute primes up to `limit` and fill `candidate` with primes whose
   reversal is divisible by 19.  Stop once `need` candidates have been
   collected. */
static void recompute_candidates(uint64_t limit, size_t need)
{
    /* Allocate sieve bitmap */
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!is_prime) exit(1); /* out of memory */
    memset(is_prime, 1, limit + 1);
    is_prime[0] = is_prime[1] = 0;

    /* Eratosthenes sieve */
    uint64_t i;
    for (i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Clear previous candidates */
    candidate_cnt = 0;

    /* Collect candidates */
    for (i = 2; i <= limit && candidate_cnt < need; ++i)
    {
        if (!is_prime[i]) continue;

        /* reverse modulo 19 */
        uint64_t rev_mod = 0;
        uint64_t x = i;
        while (x)
        {
            rev_mod = (rev_mod * 10 + x % 10) % 19;
            x /= 10;
        }
        if (rev_mod == 0)
            append_candidate(i);
    }

    free(is_prime);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for n=0, but return 0 */
        return (seq_t)0;

    /* Initialise limit if first call */
    if (current_limit == 0)
        current_limit = 100000; /* start with a modest range */

    /* Ensure we have at least n candidates */
    while (candidate_cnt < (size_t)n)
    {
        recompute_candidates(current_limit, (size_t)n);
        if (candidate_cnt < (size_t)n)
            current_limit *= 2; /* increase range and try again */
    }

    uint64_t result = candidate[n - 1];
    return (seq_t)result;
}
