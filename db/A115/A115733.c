/*
 * A115733  prime(n)-n, repeated (prime(n)-n) times.
 * https://oeis.org/A115733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = prime(k)-k where k is the smallest integer such that
 *            Σ_{i=1}^{k} (prime(i)-i) ≥ n. The sequence consists of each
 *            difference repeated as many times as the difference itself
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Prime cycle cache */
static uint64_t *prime_cache = NULL;
static idx_t *cum_cache     = NULL;
static size_t prime_cap     = 0;
static size_t cum_cap       = 0;
static size_t prime_cnt     = 0;   /* number of primes cached         */
static size_t cum_cnt       = 0;   /* number of cumulative counts      */

static void grow_primes(size_t new_cap) {
    if (new_cap <= prime_cap) return;
    size_t new_size = prime_cap ? prime_cap * 2 : 16;
    while (new_size < new_cap) new_size *= 2;
    prime_cache = (uint64_t *)realloc(prime_cache, new_size * sizeof(uint64_t));
    if (!prime_cache) abort();
    prime_cap = new_size;
}

static void grow_cum(size_t new_cap) {
    if (new_cap <= cum_cap) return;
    size_t new_size = cum_cap ? cum_cap * 2 : 16;
    while (new_size < new_cap) new_size *= 2;
    cum_cache = (idx_t *)realloc(cum_cache, new_size * sizeof(idx_t));
    if (!cum_cache) abort();
    cum_cap = new_size;
}

/* Append next prime and its cumulative block length. */
static void append_next_prime(void) {
    idx_t candidate;
    if (prime_cnt == 0) {
        candidate = 2;
    } else {
        candidate = prime_cache[prime_cnt - 1] + 1;
        if ((candidate & 1) == 0) candidate++;     /* skip even */
    }
    while (!is_prime(candidate)) {
        candidate += 2;
    }

    grow_primes(prime_cnt + 1);
    prime_cache[prime_cnt] = (uint64_t)candidate;

    seq_t d = (seq_t)candidate - (seq_t)(prime_cnt + 1);

    idx_t cum = (idx_t)d;
    if (cum_cnt > 0) cum += cum_cache[cum_cnt - 1];

    grow_cum(cum_cnt + 1);
    cum_cache[cum_cnt] = cum;

    prime_cnt++;
    cum_cnt++;
}

/* Ensure that cum_cache contains an entry ≥ n (1‑based). */
static void ensure_up_to_n(idx_t n) {
    while (cum_cnt == 0 || cum_cache[cum_cnt - 1] < n) {
        append_next_prime();
    }
}

/* Binary search: first index i with cum_cache[i] ≥ n. */
static idx_t find_block(idx_t n) {
    idx_t lo = 0, hi = cum_cnt;
    while (lo < hi) {
        idx_t mid = (lo + hi) / 2;
        if (cum_cache[mid] >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;   /* 0‑based block index */
}

/* External function required by the harness. */
seq_t a(idx_t n) {
    if (n <= 0) return 0;                  /* offset is 1; behaviour below 1 is irrelevant */
    ensure_up_to_n(n);
    idx_t block_idx = find_block(n);       /* 0‑based block index */
    seq_t value = (seq_t)prime_cache[block_idx] - (seq_t)(block_idx + 1);
    return value;
}
