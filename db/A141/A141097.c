/*
 * A141097  Number of unordered pairs of coprime composite numbers that sum to
 *          2n.
 * https://oeis.org/A141097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs of coprime composite numbers
 *            whose sum is 2n
 * Range:     u8 n <= 1101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a dynamic sieve of Eratosthenes up to the required
   value 2n.  It stores primes as 1 and non‑primes as 0.  The sieve is rebuilt
   only when a larger 2n is requested.  Counting is done by iterating over
   the smaller element of each unordered pair. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../utils/gcd.h"

/* Dynamic sieve storage */
static uint8_t *is_prime = NULL;
static idx_t sieve_limit = 0;

/* Simple integer square root – sufficient for 32‑bit limits used here */
static idx_t int_sqrt(idx_t n) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* Extend or rebuild the sieve to cover all integers up to limit */
static void ensure_sieve(idx_t limit) {
    if (limit <= sieve_limit)
        return;

    /* Allocate fresh array */
    uint8_t *new_arr =
        (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!new_arr)
        return;   /* allocation failure – unlikely for the problem size */

    /* Initialize all entries as prime */
    memset(new_arr, 1, (limit + 1) * sizeof(uint8_t));

    /* 0 and 1 are not prime */
    if (limit >= 0) new_arr[0] = 0;
    if (limit >= 1) new_arr[1] = 0;

    /* Sieve of Eratosthenes */
    idx_t root = int_sqrt(limit);
    for (idx_t p = 2; p <= root; ++p) {
        if (new_arr[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                new_arr[m] = 0;
        }
    }

    /* Replace old array */
    free(is_prime);
    is_prime = new_arr;
    sieve_limit = limit;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;

    idx_t target = 2ULL * n;          /* 2n */
    ensure_sieve(target);

    seq_t count = 0;

    for (idx_t x = 4; x <= n; ++x) {   /* x is the smaller composite of the pair */
        if (is_prime[x])               /* x must be composite */
            continue;

        idx_t y = target - x;
        if (y < x)
            break;                     /* unordered pairs, stop when x > y */

        if (is_prime[y])               /* y must also be composite */
            continue;

        if (gcd((seq_t)x, (seq_t)y) == 1)
            ++count;
    }

    return count;
}
