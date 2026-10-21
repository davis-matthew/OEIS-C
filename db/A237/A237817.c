/*
 * A237817  Number of primes p < n such that r = |{q <= n-p: q and q + 2 are
 *          both prime}| and r + 2 are both prime.
 * https://oeis.org/A237817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p < n such that r = #{ q ≤ n‑p : q and
 *            q+2 are both prime } and r and r+2 are both prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

static idx_t sieve_max = 0;
static bool *prime_flag = NULL;        /* prime_flag[i] == true if i is prime */
static idx_t *prime_list = NULL;       /* sorted list of primes up to sieve_max */
static size_t prime_count = 0;
static seq_t *twin_prefix = NULL;      /* twin_prefix[i] = #{ q ≤ i : q and q+2 prime } */

/* Rebuild sieve and twin prefix up to at least "limit" (inclusive). */
static void recompute(idx_t limit) {
    if (limit <= sieve_max) return;

    if (prime_flag) free(prime_flag);
    if (prime_list) free(prime_list);
    if (twin_prefix) free(twin_prefix);

    sieve_max = limit;
    prime_flag = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!prime_flag) exit(1);

    /* Initialise all numbers as potentially prime. */
    for (idx_t i = 0; i <= limit; ++i) prime_flag[i] = true;
    if (limit >= 0) prime_flag[0] = false;
    if (limit >= 1) prime_flag[1] = false;

    /* Eratosthenes sieve. */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_flag[j] = false;
        }
    }

    /* Pack primes into prime_list. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_flag[i]) ++cnt;
    prime_count = cnt;
    prime_list = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!prime_list) exit(1);
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_flag[i]) prime_list[k++] = i;

    /* Build twin‑prime prefix sums. */
    twin_prefix = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
    if (!twin_prefix) exit(1);
    twin_prefix[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t add = 0;
        if (i >= 2 && prime_flag[i]) {
            if (i + 2 <= limit && prime_flag[i + 2]) add = 1;
        }
        twin_prefix[i] = twin_prefix[i - 1] + add;
    }
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;

    idx_t limit = n + 2;          /* need primes up to n+2 for q+2 checks */
    recompute(limit);

    seq_t result = 0;
    for (size_t idx = 0; idx < prime_count; ++idx) {
        idx_t p = prime_list[idx];
        if (p >= n) break;
        idx_t m = n - p;
        seq_t r = twin_prefix[m];
        if (is_prime_small(r) && is_prime_small(r + 2))
            ++result;
    }
    return result;
}
