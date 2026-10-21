/*
 * A074124  Group the composite numbers so that the sum of the n-th group is a
 *          multiple of the n-th prime: (4), (6), (8, 9, 10, 12, 14, 15, 16,
 *          18, 20, 21, 22), (24, 25), (26, 27, 28, 30, 32), (33, 34, ...),
 *          ... Sequence gives sum of n-th group.
 * https://oeis.org/A074124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A074124: a(n) is the sum of the n-th group of composite numbers,
 * where the n-th group is the consecutive unused composite numbers
 * whose sum is a multiple of the n-th prime.
 *
 * The algorithm generates primes on the fly and builds the groups sequentially,
 * caching the sums for memoization.  It satisfies the requirements on types
 * and operations, and is written to compile both in C11 and C++17.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime generation ------------------------------------ */

static idx_t *prime_tbl = NULL;   /* table of found primes */
static size_t prime_cap = 0;      /* allocated capacity */
static size_t prime_len = 0;      /* number of stored primes */
static idx_t prime_next_candidate = 2;  /* candidate for the next prime */

/* Grow the prime table as needed. */
static void grow_prime_array(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    prime_tbl = (idx_t *)realloc(prime_tbl, new_cap * sizeof(idx_t));
    prime_cap = new_cap;
}

/* Test whether x is a prime using the primes that have been found. */
static bool is_candidate_prime(idx_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        idx_t p = prime_tbl[i];
        if (p > x / p) break;         /* p*p > x */
        if (x % p == 0) return false;
    }
    return true;
}

/* Generate and return the next prime number. */
static idx_t next_prime(void)
{
    while (!is_candidate_prime(prime_next_candidate)) ++prime_next_candidate;
    idx_t p = prime_next_candidate;
    ++prime_next_candidate;
    grow_prime_array(prime_len + 1);
    prime_tbl[prime_len++] = p;
    return p;
}

/* Return the n-th prime (1‑based), generating more if necessary. */
static idx_t nth_prime(idx_t n)
{
    while (prime_len < n) next_prime();
    return prime_tbl[n - 1];
}

/* ---------- Group computation ----------------------------------- */

static seq_t *sum_cache = NULL;   /* cached sums of groups */
static size_t cache_cap = 0;
static size_t cache_len = 0;

static idx_t composite_next = 4;  /* next composite to consider */

/* Grow the cache array if needed. */
static void grow_sum_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    sum_cache = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Ensure that primes up to at least "limit" are available.  Used before
 * checking composite numbers so that is_candidate_prime does not misclassify
 * small composites. */
static void ensure_prime_limit(idx_t limit)
{
    while (prime_next_candidate <= limit) next_prime();
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Make sure at least the first few primes exist, so that
       is_candidate_prime correctly handles small composites. */
    ensure_prime_limit(3);

    /* Compute groups up to n, storing sums in cache. */
    while (cache_len < n) {
        idx_t group_index = cache_len + 1;          /* groups are 1‑based */

        /* Prime needed for this group. */
        idx_t p = nth_prime(group_index);

        /* Build the group. */
        seq_t sum = 0;
        for (;;) {
            /* Skip over any primes until we hit a composite. */
            while (is_candidate_prime(composite_next)) ++composite_next;

            /* Now composite_next is composite. */
            sum += (seq_t)composite_next;

            /* Stop when the sum is a multiple of the required prime. */
            if ((sum % (seq_t)p) == 0) break;

            ++composite_next;
        }

        /* The next group will start with the next composite after the current one. */
        ++composite_next;

        /* Store the group's sum. */
        grow_sum_cache(cache_len + 1);
        sum_cache[cache_len++] = sum;
    }

    return sum_cache[n - 1];
}
