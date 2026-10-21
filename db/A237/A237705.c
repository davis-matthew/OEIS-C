/*
 * A237705  Number of primes p < n with pi(n-p) prime, where pi(.) is given by
 *          A000720.
 * https://oeis.org/A237705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2634, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A237705: a(n) = # of primes p < n such that pi(n‑p) is prime.
   The routine builds an Eratosthenes sieve up to the largest n requested
   and stores a prefix sum array for the prime counting function pi(k).
   For each prime p < n it checks whether pi(n‑p) is itself a prime.
   The computation is O(n log log n) for the sieve and then linear in the
   number of primes below n per call, which is comfortably fast for n ≤ 10000. */
#include <stdlib.h>
#include <stdbool.h>

/* static state for the sieve and pi table */
static bool *prime_sieve = NULL;        /* prime_sieve[i] == true if i is prime */
static seq_t *pi_cache   = NULL;        /* pi_cache[i] = number of primes <= i */
static size_t sieve_cap  = 0;            /* current capacity of prime_sieve/pi_cache */

/* Ensure that the arrays have capacity at least n+1.  If the capacity
   must be increased, recompute the sieve and the prefix sums from
   scratch.  This is acceptable for the small sizes needed. */
static void ensure_sieve(idx_t n)
{
    if (n + 1 <= sieve_cap)
        return;                       /* already large enough */

    size_t new_cap = n + 1;
    if (new_cap < 16)
        new_cap = 16;

    /* allocate or extend the arrays, casting to avoid C++ conversion errors */
    prime_sieve = (bool *)realloc(prime_sieve, new_cap * sizeof(bool));
    pi_cache    = (seq_t *)realloc(pi_cache,    new_cap * sizeof(seq_t));

    /* initialize all entries to true, then sieve out composites */
    for (size_t i = 0; i < new_cap; ++i)
        prime_sieve[i] = true;
    if (new_cap > 0) prime_sieve[0] = false;
    if (new_cap > 1) prime_sieve[1] = false;

    for (size_t i = 2; i * i < new_cap; ++i) {
        if (prime_sieve[i]) {
            for (size_t j = i * i; j < new_cap; j += i)
                prime_sieve[j] = false;
        }
    }

    /* build the prefix sum array for pi(k) */
    pi_cache[0] = 0;
    for (size_t i = 1; i < new_cap; ++i) {
        pi_cache[i] = pi_cache[i - 1] + (prime_sieve[i] ? 1 : 0);
    }

    sieve_cap = new_cap;
}

/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n <= 4)                     /* a(1)=a(2)=a(3)=a(4)=0 */
        return 0;

    /* Ensure arrays are large enough to include all values up to n‑1 */
    ensure_sieve(n - 1);

    seq_t count = 0;
    for (idx_t p = 2; p < n; ++p) {
        if (prime_sieve[p]) {                    /* p is prime */
            idx_t q = n - p;                     /* q = n - p */
            seq_t pi_q = pi_cache[q];            /* pi(n-p) */
            if (pi_q >= 2 && prime_sieve[(idx_t)pi_q]) {
                ++count;                         /* pi_q is prime */
            }
        }
    }
    return count;
}
