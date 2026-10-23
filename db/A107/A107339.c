/*
 * A107339  Longest sequence of n-digit n-almost primes formed by beginning
 *          with a single-digit prime and appending a single decimal digit to
 *          each previous term.
 * https://oeis.org/A107339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching for the longest chain of n‑digit n‑almost primes. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Global primes storage for factorization */
#define PRIME_LIMIT 1000000
/* A 32‑bit array large enough for primes below 1e6 (78498 primes) */
static uint32_t primes[80000];
static int prime_cnt = 0;
static bool primes_computed = false;
static void compute_primes(void)
{
    if (primes_computed) return;
    bool *is_prime = (bool *)malloc(PRIME_LIMIT + 1);
    for (int i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            primes[prime_cnt++] = (uint32_t)i;
    free(is_prime);
    primes_computed = true;
}

/* Count prime factors (with multiplicity) for an unsigned 64‑bit integer. */
static int count_prime_factors_u64(uint64_t n)
{
    if (n <= 1) return 0;
    int cnt = 0;
    for (int i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        while (n % p == 0) {
            n /= p;
            ++cnt;
        }
    }
    if (n > 1) ++cnt;
    return cnt;
}

/* Variables to keep the best chain. */
static seq_t best_chain[200];
static int best_len = 0;
static seq_t path[200];

/* Recursive DFS that builds the chain. */
static void dfs_seq(seq_t current, int depth)
{
    /* depth is the number of digits of 'current' */
    path[depth - 1] = current;

    if (depth > best_len) {
        best_len = depth;
        for (int i = 0; i < best_len; ++i) best_chain[i] = path[i];
    }

    /* Try appending each decimal digit */
    for (int d = 0; d <= 9; ++d) {
        seq_t next = current * (seq_t)10 + (seq_t)d;
        int next_len = depth + 1;

        /* Cast to uint64_t for factor counting; if overflow, skip */
        uint64_t val = (uint64_t)next;
        if ((seq_t)val != next) continue; /* overflow, not needed for chain */

        if (count_prime_factors_u64(val) == next_len) {
            dfs_seq(next, next_len);
        }
    }
}

/* Public function required by the specification. */
extern seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        compute_primes();
        /* Start the search from all single‑digit primes */
        const uint32_t starts[] = {2, 3, 5, 7};
        for (int i = 0; i < 4; ++i) {
            dfs_seq((seq_t)starts[i], 1);
        }
        initialized = true;
    }

    if (n == 0 || n > (idx_t)best_len) return (seq_t)0;
    return best_chain[(idx_t)n - 1];
}
