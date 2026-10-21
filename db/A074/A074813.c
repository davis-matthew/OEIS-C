/*
 * A074813  Number of primes between n and R(n) (inclusive), where R(n)
 *          (A004086) is the digit reversal of n.
 * https://oeis.org/A074813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of primes between n and rev(n) (inclusive)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/rev.h"     /*  rev(idx_t)  */

static bool *prime_sieve = NULL;   /*  prime_sieve[i] == true <=> i is prime (0‑based) */
static idx_t sieve_limit   = 0;     /*  maximum number for which the sieve is built */

/* Integer square‑root (floor) using binary search.  */
static idx_t isqrt_uint64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid   = (low + high) >> 1;
        idx_t sqr   = mid * mid;
        if (sqr == n) return mid;
        if (sqr < n) {
            low = mid + 1;
            /* avoid overflow for (mid+1)^2 */
            if (mid + 1 > n / (mid + 1)) {
                high = mid;
                break;
            }
        } else {
            high = mid - 1;
        }
    }
    return low - 1;
}

/* Ensure the sieve covers the range 0 … limit.  */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* Allocate a new sieve large enough.  */
    bool *new_sieve = (bool *)calloc(limit + 1, sizeof(bool));
    if (!new_sieve) exit(1);    /* OOM */

    /* Assume all numbers ≥2 are prime initially.  */
    for (idx_t i = 2; i <= limit; ++i) new_sieve[i] = true;

    /* Eratosthenes sieve.  */
    idx_t sq = isqrt_uint64(limit);
    for (idx_t p = 2; p <= sq; ++p) {
        if (new_sieve[p]) {
            /* start at p*p, but guard against overflow on large limits */
            idx_t start = p * p;
            for (idx_t mult = start; mult <= limit; mult += p) {
                new_sieve[mult] = false;
            }
        }
    }

    /* 0 and 1 are not prime.  */
    if (limit >= 0) new_sieve[0] = false;
    if (limit >= 1) new_sieve[1] = false;

    /* Replace the old sieve.  */
    free(prime_sieve);
    prime_sieve = new_sieve;
    sieve_limit = limit;
}

/* Sequence function.  */
seq_t a(idx_t n)
{
    /* Reverse decimal digits of n.  */
    idx_t rev_n = rev(n);

    /* Determine the inclusive window.  */
    idx_t low  = (n < rev_n) ? n : rev_n;
    idx_t high = (n > rev_n) ? n : rev_n;

    /* Build or extend the sieve up to 'high'.  */
    ensure_sieve(high);

    /* Count primes in [low, high].  */
    seq_t count = 0;
    for (idx_t v = low; v <= high; ++v) {
        if (prime_sieve[v]) ++count;
    }
    return count;
}
