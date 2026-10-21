/*
 * A075540  Integers that are the average of three successive primes.
 * https://oeis.org/A075540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p(i)+p(i+1)+p(i+2))/3 where p(i) are successive primes
 *            and the sum is divisible by 3
 * Range:     u8 n <= 2, u16 n <= 901, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* The number of primes and terms we are willing to keep cached.  This is
   comfortably larger than needed for all test cases (~10 000 terms). */
#define MAX_PRIMES   300000
#define MAX_TERMS    200000

/* Prime storage.  prime_table[0] = 2, prime_table[1] = 3. */
static uint64_t prime_table[MAX_PRIMES];
static size_t   prime_cnt = 0;

/* Cached 3‑averages.  term_table[0] is a(1). */
static seq_t term_table[MAX_TERMS];
static size_t term_cnt   = 0;

/* Next odd candidate for primality testing. */
static uint64_t next_candidate = 5ULL;

/* ------------------------------------------------------------------
   Test whether a 64‑bit integer is prime.  The test uses trial division
   by all primes already stored in prime_table, stopping when p*p > n.
   ------------------------------------------------------------------ */
static bool is_prime_val(uint64_t n)
{
    if (n < 2ULL)          return false;
    if (n == 2ULL)         return true;
    if ((n & 1ULL) == 0ULL)/* even numbers >2 are not prime */
        return false;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_table[i];
        if (p > n / p)     /* equivalent to p*p > n, but no overflow */
            break;
        if (n % p == 0ULL)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------
   Return the nth term of A075540 (1‑based index).  The function
   builds primes and the average sequence lazily, keeping the result
   in a static cache so repeated calls are O(1) after the initial
   build.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0ULL) return 0;           /* offset is 1 – never really used */

    /* Initialise prime list if this is the first call. */
    if (prime_cnt == 0U)
    {
        prime_table[0] = 2ULL;
        prime_table[1] = 3ULL;
        prime_cnt = 2U;
    }

    /* Generate primes and 3‑averages until we have the nth term. */
    while (term_cnt < (size_t)n)
    {
        /* Find the next prime. */
        while (!is_prime_val(next_candidate))
            next_candidate += 2ULL;      /* skip even numbers */

        /* Store the new prime. */
        if (prime_cnt < MAX_PRIMES)
            prime_table[prime_cnt++] = next_candidate;

        /* If we have at least 3 primes, compute the average of the last
           three primes. */
        if (prime_cnt >= 3U)
        {
            seq_t p1 = (seq_t)prime_table[prime_cnt - 3U];
            seq_t p2 = (seq_t)prime_table[prime_cnt - 2U];
            seq_t p3 = (seq_t)prime_table[prime_cnt - 1U];
            seq_t sum = p1 + p2 + p3;
            if ((sum % (seq_t)3U) == 0)
            {
                if (term_cnt < MAX_TERMS)
                    term_table[term_cnt++] = sum / (seq_t)3U;
            }
        }

        /* Advance to next odd candidate for the next iteration. */
        next_candidate += 2ULL;
    }

    return term_table[(size_t)(n - 1U)];
}
