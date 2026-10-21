/*
 * A225926  Least number of prime powers needed to sum up to n.
 * https://oeis.org/A225926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A225926 – least number of prime powers (p^k, k≥0 with p^0=1, excluding p^1)
 *  Needed to sum to n.  Computed by dynamic programming over the set of
 *  all prime powers ≤ n together with the value 1.  Each coin may be used
 *  any number of times (non‑distinct allowed).  The algorithm updates a
 *  DP array of size n+1: dp[i] = min(dp[i-c]+1) over all coins c≤i.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* dp[i] – minimal number of prime powers summing to i */
static seq_t *dp = NULL;
static idx_t  max_computed = 0;  /* highest index for which dp is valid */

/* coin list (prime powers, sorted ascending) */
static idx_t *coin_arr = NULL;
static size_t coin_count = 0;
static size_t coin_cap   = 0;
static idx_t  coin_max   = 0;

/* Append a coin value, growing the array if necessary. */
static void append_coin(idx_t val)
{
    if (coin_count == coin_cap) {
        size_t new_cap = (coin_cap == 0) ? 16 : coin_cap * 2;
        idx_t *tmp = (idx_t *)realloc(coin_arr, new_cap * sizeof(idx_t));
        if (!tmp) exit(EXIT_FAILURE);
        coin_arr = tmp;
        coin_cap = new_cap;
    }
    /* avoid duplicates (e.g., 1 from several primes) */
    if (coin_count == 0 || coin_arr[coin_count - 1] != val)
        coin_arr[coin_count++] = val;
}

/* Build the complete list of prime powers up to 'limit'.
 * The list is sorted increasingly. */
static void compute_coins(idx_t limit)
{
    if (limit <= coin_max && coin_arr) return;   /* already built up to this limit */

    /* free old list */
    free(coin_arr);
    coin_arr = NULL;
    coin_count = coin_cap = 0;
    coin_max = limit;

    /* 1 is always usable (p^0). */
    append_coin(1);

    /* Sieve of Eratosthenes up to limit. */
    size_t sz = (size_t)limit + 1;
    uint8_t *sieve = (uint8_t *)calloc(sz, 1);
    if (!sieve) exit(EXIT_FAILURE);
    sieve[0] = sieve[1] = 1;  /* 0 and 1 are not prime */
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;
        }
    }

    /* For each prime, generate its powers p^k for k >= 2. */
    for (idx_t p = 2; p <= limit; ++p) {
        if (sieve[p]) continue;   /* p is not prime */
        idx_t val = p * p;        /* start with p^2 */
        while (val <= limit) {
            append_coin(val);
            if (val > limit / p) break;   /* prevent overflow */
            val *= p;
        }
    }

    free(sieve);
}

/* Extend dp array and recompute values up to new_max. */
static void extend_dp(idx_t new_max)
{
    if (new_max <= max_computed) return;  /* already up to date */

    compute_coins(new_max);   /* ensure all coin values up to new_max */

    /* Resize dp array. */
    size_t new_size = (size_t)new_max + 1;
    seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    dp = tmp;

    /* Initialize dp[0] if this is the first allocation. */
    if (max_computed == 0) dp[0] = (seq_t)0;

    /* For new indices, start with worst case of all 1's. */
    for (idx_t i = max_computed + 1; i <= new_max; ++i)
        dp[i] = (seq_t)i;

    /* Dynamic programming update for each new index. */
    for (idx_t i = max_computed + 1; i <= new_max; ++i) {
        seq_t best = dp[i];
        for (size_t ci = 0; ci < coin_count && coin_arr[ci] <= i; ++ci) {
            idx_t c = coin_arr[ci];
            seq_t cand = dp[i - c] + (seq_t)1;
            if (cand < best) best = cand;
        }
        dp[i] = best;
    }

    max_computed = new_max;
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n=0, but handle safely */
    extend_dp(n);
    return dp[n];
}
