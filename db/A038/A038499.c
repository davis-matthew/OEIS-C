/*
 * A038499  Number of partitions of n into a prime number of parts.
 * https://oeis.org/A038499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 49, u32 n <= 140, u64 n <= 441,
 *            u128 n <= 1508, big any n
 * Helpers:   alloc_seq_array, build_primes_upto, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A038499: Number of partitions of n into a prime number of parts.
   The computation uses a dynamic‑programming approach that updates the
   partition table incrementally.  For each part size i the table dp[s]
   counts partitions of s using parts ≤ i.  When i is prime, each new
   partition of s containing i contributes to a(s); this is captured
   by ans[s] += dp[s-i] after the DP update for that i.  The algorithm
   runs in O(n²) time and O(n) memory, and is memoised so that multiple
   calls to a() in any order are handled efficiently.                                                     */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/build_primes_upto.h"

/* static state for memoised computation */
static seq_t *dp = NULL;      /* partition counts for current N */
static seq_t *ans = NULL;     /* a(n) values for current N */
static idx_t maxN = 0;        /* current maximum n for which tables are built */

/* Allocate/resize DP and result tables up to N (inclusive).  All entries
   are initialised to zero except dp[0] and ans[0] which are set to 1. */
static void recompute(idx_t N)
{
    /* free previous tables, if any */
    if (dp) free_seq_array(dp);
    if (ans) free_seq_array(ans);

    /* allocate fresh arrays */
    dp  = alloc_seq_array((size_t)N + 1);
    ans = alloc_seq_array((size_t)N + 1);

    /* initialise tables */
    memset(dp,  0, sizeof(seq_t) * ((size_t)N + 1));
    memset(ans, 0, sizeof(seq_t) * ((size_t)N + 1));

    /* dp[0] = 1   (empty partition) */
    dp[0] = (seq_t)1;
    /* ans[0] = 1   (a(0) is defined to be 1) */
    ans[0] = (seq_t)1;

    /* generate prime information up to N by a simple sieve */
    bool *is_prime = (bool*)calloc((size_t)N + 1, sizeof(bool));
    if (!is_prime) abort();
    for (idx_t i = 0; i <= N; ++i) is_prime[i] = true;
    if (N >= 0) is_prime[0] = false;
    if (N >= 1) is_prime[1] = false;
    for (idx_t i = 2; i * i <= N; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= N; j += i) is_prime[j] = false;
        }
    }

    /* dynamic–programming loops */
    for (idx_t i = 1; i <= N; ++i) {
        /* add part i to all sums */
        for (idx_t s = i; s <= N; ++s) {
            dp[s] += dp[s - i];
        }
        /* if i is prime, accumulate contributions to ans */
        if (is_prime[i]) {
            for (idx_t s = i; s <= N; ++s) {
                ans[s] += dp[s - i];
            }
        }
    }

    free(is_prime);
    maxN = N;
}

/* external interface required by the harness */
seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0) return (seq_t)1;

    if (n > maxN) recompute(n);
    return ans[n];
}
