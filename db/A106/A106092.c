/*
 * A106092  Even numbers and primes.
 * https://oeis.org/A106092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106092 – Even numbers and primes
   Formula: a(n) = min{ x ≥ 2 : floor(x/2) + π(x) – 1 ≥ n }
   where π(x) is the number of primes ≤ x. This counts all even numbers
   plus all odd primes.  The sequence is therefore the list of all
   numbers ≥ 2 that are either even or prime.  The function uses a
   simple adaptive sieve to obtain π(x) and performs a binary search
   for each requested index.  It is valid for any n ≥ 1 and works
   for the built‑in arbitrary‑precision, 64‑bit, and lower‑precision
   integer types. */

#include <stdlib.h>
#include <stdint.h>

/* Global state for the adaptive sieve. */
static idx_t    sieve_max   = 0;                 /* largest value already sieved */
static unsigned char *is_prime = NULL;           /* 0/1 flag for primality */
static idx_t    *prime_cnt = NULL;               /* prefix count of primes */

/* ------------------------------------------------------------------
   Ensure that the sieve has been computed up to at least `limit`.
   If `limit` exceeds the current size, reallocate larger buffers
   and perform a full sieve up to `limit`.  For the small limits
   needed here (≤ ~40000) the cost is negligible.
   ------------------------------------------------------------------ */
static void sieve_up_to(idx_t limit)
{
    if (limit <= sieve_max) return;            /* already sufficient */

    /* Allocate new buffers */
    unsigned char *new_prime = (unsigned char *)realloc(is_prime,
                                 (size_t)(limit + 1));
    if (!new_prime) exit(EXIT_FAILURE);
    is_prime = new_prime;

    idx_t *new_cnt = (idx_t *)realloc(prime_cnt,
                           (size_t)(limit + 1) * sizeof(idx_t));
    if (!new_cnt) exit(EXIT_FAILURE);
    prime_cnt = new_cnt;

    /* Initialise all as prime */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (idx_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;

    /* Prefix counts of primes */
    idx_t count = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (is_prime[i]) ++count;
        prime_cnt[i] = count;
    }

    sieve_max = limit;
}

/* ------------------------------------------------------------------
   Return the n-th term of A106092 (with offset 1).
   The function uses the formula described in the comment block.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n < 1 */

    /* Estimate an upper bound for the search interval.  2*n is always
       sufficient because floor(x/2) ≥ n for x = 2*n.  A small safety
       margin avoids problems for very small n. */
    idx_t hi = 2 * n + 100;
    if (hi < 4) hi = 4;                       /* ensure at least 4 */

    /* Make sure the sieve covers the entire interval needed. */
    sieve_up_to(hi);

    /* Binary search for the minimal x with the required count. */
    idx_t lo = 2;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;

        /* Count evens up to mid: floor(mid/2). */
        idx_t evens = mid / 2;

        /* Count odd primes up to mid: π(mid) – 1 (remove the prime 2). */
        idx_t primes = prime_cnt[mid];
        if (mid >= 2) primes -= 1;            /* exclude 2 */
        else primes = 0;

        idx_t total = evens + primes;
        if (total >= n)
            hi = mid;
        else
            lo = mid + 1;
    }

    return (seq_t)lo;
}
