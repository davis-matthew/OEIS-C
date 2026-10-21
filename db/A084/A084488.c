/*
 * A084488  Prime(n)+q, where q is the next prime such that q mod 4 =
 *          (prime(n)+2) mod 4.
 * https://oeis.org/A084488
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prime(n) + next prime q > Prime(n) with q mod 4 =
 *            (Prime(n)+2) mod 4
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   build_primes, estimate_upper_for_n, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses utility functions build_primes, estimate_upper_for_n and next_prime */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes.h"          /* void build_primes(idx_t limit, idx_t **out, size_t *cnt); */
#include "../utils/estimate_upper_for_n.h"  /* size_t estimate_upper_for_n(size_t n); */
#include "../utils/next_prime.h"            /* uint64_t next_prime(uint64_t start); */

/* Static storage for primes */
static idx_t *prime_list = NULL;   /* array of primes, 1‑indexed logically */
static size_t prime_count = 0;     /* number of primes stored */

/* Ensure that at least 'needed' primes are available */
static void ensure_primes(size_t needed)
{
    if (prime_count >= needed)
        return;

    /* Estimate an upper bound large enough to contain 'needed' primes */
    idx_t upper = estimate_upper_for_n(needed + 10);
    size_t cnt;
    idx_t *tmp = NULL;

    /* Reallocate the prime list for the new upper bound */
    if (prime_list)
        free(prime_list);

    build_primes(upper, &tmp, &cnt);
    prime_list  = tmp;
    prime_count = cnt;
}

seq_t a(idx_t n)
{
    /* Offset 2: a(2) is the first defined term */
    if (n < 2)
        return 0;

    /* Make sure we have the n-th prime */
    ensure_primes((size_t)n);

    /* Retrieve Prime(n) */
    seq_t p = (seq_t)prime_list[n - 1];

    /* Desired mod 4 value for the following prime */
    int target_mod = (int)((p + 2) & 3);   /* (Prime(n)+2) mod 4 */

    /* Find the next prime with the required mod 4 */
    idx_t candidate = (idx_t)p + 1;   /* start search after Prime(n) */

    while (1) {
        candidate = next_prime(candidate);  /* next prime > candidate */
        if ((candidate & 3) == target_mod)
            break;
    }

    seq_t q = (seq_t)candidate;
    return p + q;
}
