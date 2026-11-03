/*
 * A116358  Numbers that cannot be written as sum of products of two
 *          successive primes.
 * https://oeis.org/A116358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = numbers that cannot be written as sum of products of two
 *            successive primes
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: full, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* internal helper: recompute missing values up to 'limit'.
   The result is stored in '*out_missing' and the count in '*out_count'.
   The caller must free '*out_missing' after use. */
static bool _recompute_missing(idx_t limit, seq_t **out_missing, size_t *out_count)
{
    /* Allocate array for prime sieve */
    bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime) return false;
    if (limit >= 0) {
        for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;
    }

    /* Temporary array for primes */
    size_t primes_cap = 128;
    uint64_t *primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    if (!primes) { free(is_prime); return false; }
    size_t primes_cnt = 0;

    /* Simple sieve to collect primes up to limit */
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) { free(is_prime); free(primes); return false; }
                primes = tmp;
            }
            primes[primes_cnt++] = i;

            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    is_prime[j] = false;
                }
            }
        }
    }

    /* Build list of coin denominations = p_i * p_{i+1}, only up to limit */
    size_t coins_cap = primes_cnt;
    uint64_t *coins = (uint64_t *)malloc(coins_cap * sizeof(uint64_t));
    if (!coins) { free(is_prime); free(primes); return false; }
    size_t coins_cnt = 0;
    for (size_t k = 0; k + 1 < primes_cnt; ++k) {
        uint64_t prod = primes[k] * primes[k + 1];
        if (prod <= limit) {
            if (coins_cnt == coins_cap) {
                coins_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(coins, coins_cap * sizeof(uint64_t));
                if (!tmp) { free(is_prime); free(primes); free(coins); return false; }
                coins = tmp;
            }
            coins[coins_cnt++] = prod;
        } else break;
    }

    /* Dynamic programming: compute reachable sums up to 'limit' */
    unsigned char *reachable = (unsigned char *)calloc((size_t)(limit + 1), sizeof(unsigned char));
    if (!reachable) { free(is_prime); free(primes); free(coins); return false; }
    reachable[0] = 1;  /* base case */

    for (size_t ci = 0; ci < coins_cnt; ++ci) {
        uint64_t coin = coins[ci];
        for (idx_t i = coin; i <= limit; ++i) {
            if (reachable[(size_t)(i - coin)]) reachable[(size_t)i] = 1;
        }
    }

    /* Collect missing numbers */
    seq_t *missing = mem_new(seq_t, (size_t)(limit + 1));
    if (!missing) { free(is_prime); free(primes); free(coins); free(reachable); return false; }
    size_t miss_cnt = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (!reachable[(size_t)i]) {
            missing[miss_cnt++] = (seq_t)i;
        }
    }

    /* Clean up temporary arrays */
    free(is_prime);
    free(primes);
    free(coins);
    free(reachable);

    *out_missing = missing;
    *out_count   = miss_cnt;
    return true;
}

/* Cached data for successive calls */
static idx_t          _current_limit   = 0;
static seq_t *        _missing_vals    = NULL;
static size_t         _missing_count   = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; n=0 never requested */

    /* If cache insufficient, extend it */
    if (_missing_count < (size_t)n) {
        idx_t limit = (_current_limit > 0) ? _current_limit : (n * 2 + 10);
        while (true) {
            seq_t *new_missing = NULL;
            size_t new_count   = 0;
            if (!_recompute_missing(limit, &new_missing, &new_count)) {
                /* on failure return 0; sequence not defined */
                if (_missing_vals) mem_free(_missing_vals);
                _missing_vals   = NULL;
                _missing_count  = 0;
                _current_limit  = 0;
                return 0;
            }
            if (_missing_vals) mem_free(_missing_vals);
            _missing_vals  = new_missing;
            _missing_count = new_count;
            _current_limit = limit;
            if (_missing_count >= (size_t)n) break;
            limit <<= 1;  /* double the limit and retry */
        }
    }

    return _missing_vals[n - 1];
}
