/*
 * A116357  Number of partitions of n into products of two successive primes
 *          (A006094).
 * https://oeis.org/A116357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 510, u16 n <= 1692, u32 n <= 7833, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of partitions of n into numbers of the form
 * p_k * p_{k+1} where p_k and p_{k+1} are consecutive primes (OEIS A116357).
 */
#include <stdlib.h>
#include <stdint.h>

/* Static storage for dynamic programming and auxiliary data. */
static seq_t *dp        = NULL;    /* dp[0..current_max_n]  */
static size_t dp_size   = 0;       /* number of allocated elements in dp */

static idx_t current_max_n = 0;    /* largest n that has been computed */

/* Prime list and helper data. */
static uint32_t *primes = NULL;    /* stored primes*/
static size_t   primes_len = 0;    /* number of primes stored */
static size_t   primes_cap = 0;    /* allocated capacity for primes*/
static idx_t    primes_max  = 0;    /* last prime index considered */

/* Parts list: products p_k * p_{k+1} that are <= n. */
static idx_t *parts      = NULL;    /* stored part sizes */
static size_t parts_len  = 0;       /* number of parts stored */
static size_t parts_cap  = 0;       /* allocated capacity for parts */

/* Append a prime to the primes array, resizing if necessary. */
static void append_prime(uint32_t p)
{
    if (primes_len == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        if (!tmp) exit(1);
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = p;
}

/* Ensure that all primes up to 'limit' have been generated.
 *  The routine uses a simple incremental trial division.
 *  Numbers < 2 are skipped. */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= primes_max) return;

    for (idx_t cand = primes_max + 1; cand <= limit; ++cand) {
        if (cand < 2) continue;
        int is_prime = 1;
        for (size_t i = 0; i < primes_len; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * (uint64_t)p > cand) break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) append_prime((uint32_t)cand);
    }
    primes_max = limit;
}

/* Append a part size (product of two consecutive primes) to the parts array. */
static void append_part(idx_t val)
{
    if (parts_len == parts_cap) {
        size_t new_cap = parts_cap ? parts_cap * 2 : 16;
        idx_t *tmp = (idx_t *)realloc(parts, new_cap * sizeof(idx_t));
        if (!tmp) exit(1);
        parts = tmp;
        parts_cap = new_cap;
    }
    parts[parts_len++] = val;
}

/* Recompute dp[0..n] using all part sizes <= n. */
static void recompute_up_to(idx_t n)
{
    /* Make sure the prime list is long enough to generate all relevant products. */
    ensure_primes_up_to(n);

    /* Build the list of part sizes p_k * p_{k+1} <= n. */
    parts_len = 0;
    for (size_t i = 0; i + 1 < primes_len; ++i) {
        uint64_t prod = (uint64_t)primes[i] * (uint64_t)primes[i + 1];
        if (prod > (uint64_t)n) break;
        append_part((idx_t)prod);
    }

    /* Allocate or resize the dp array. */
    if (dp_size < (size_t)(n + 1)) {
        seq_t *tmp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        dp = tmp;
        dp_size = n + 1;
    }

    /* Base case */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* Classic unbounded coin‑change DP (order-independent). */
    for (size_t pi = 0; pi < parts_len; ++pi) {
        idx_t part = parts[pi];
        for (idx_t j = part; j <= n; ++j) {
            dp[j] += dp[j - part];
        }
    }
    current_max_n = n;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    if (n > current_max_n) {
        recompute_up_to(n);
    }
    return dp[n];
}
