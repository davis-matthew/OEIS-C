/*
 * A093933  a(1) = 3; for n >= 1, a(n+1) is the least odd prime not already
 *          used such that abs(a(n+1)-a(n)) is a multiple of prime(n).
 * https://oeis.org/A093933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 253, u32 n <= 911, u64 n <= 911,
 *            u128 n <= 911, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A093933
   a(1) = 3;
   For n ≥ 1, a(n+1) is the least odd prime not already used such that
   |a(n+1) – a(n)| is a multiple of prime(n).
   The sequence terminates at n = 911 with a(911) = 7109.  This implementation
   builds the list of all odd primes up to a fixed sieve limit and then
   generates the sequence on demand, caching computed terms for fast future
   queries.  The algorithm follows the recursive definition directly. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */

static uint64_t *prime_list   = NULL;   /* list of all primes ≤ LIMIT */
static size_t   prime_count  = 0;        /* number of primes in prime_list */
static bool    *used_flag    = NULL;   /* used_flag[i] == true iff prime_list[i] has been used in the sequence */
static seq_t   *cache_seq    = NULL;   /* cached a(1)…a(k) values */
static size_t   cache_len    = 0;        /* number of terms currently stored in cache_seq */
static size_t   cache_cap    = 0;        /* allocated capacity of cache_seq */
static bool     initialized = false;

/* Initialise primes, used flags and the initial term a(1)=3. */
static void init_sequence(void)
{
    if (initialized) return;

    /* Choose a sieve limit that comfortably contains all primes needed for
       the sequence up to the known termination point. */
    const size_t SIEVE_LIMIT = 4000000;
    build_primes_upto((idx_t)SIEVE_LIMIT, &prime_list, &prime_count);

    /* Allocate array indicating which primes have already been taken. */
    used_flag = (bool *)calloc(prime_count, sizeof(bool));
    if (!used_flag) abort();

    /* Allocate cache for the sequence values.  The sequence has 911 terms,
       so a capacity of 1024 provides a small buffer beyond that. */
    cache_cap = 1024;
    cache_seq = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache_seq) abort();

    /* Store the first term a(1)=3 and mark prime 3 as used. */
    cache_seq[0] = (seq_t)3;
    cache_len   = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        if (prime_list[i] == 3) {
            used_flag[i] = true;
            break;
        }
    }

    initialized = true;
}

/* Ensure that cache_seq contains at least the first n terms of the sequence
   (1‑based).  If n exceeds the current length, compute terms until either
   the sequence terminates or the requested index is reached. */
static void ensure_up_to(idx_t n)
{
    init_sequence();

    if ((idx_t)cache_len >= n) return;  /* already have enough terms */

    /* Grow the cache array lazily if necessary. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache_seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache_seq = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms sequentially until we reach n or the sequence ends. */
    while (cache_len < (size_t)n) {
        /* Current index (1‑based) of the last term stored in cache_seq. */
        const size_t cur_index = cache_len;          /* cur_index ≥ 1 */
        const seq_t  cur_val   = cache_seq[cur_index - 1];
        const seq_t  divisor   = (seq_t)prime_list[cur_index - 1];  /* prime(cur_index) */

        seq_t next_val = 0;

        /* Search odd primes in increasing order for the next unused one
           satisfying the divisibility condition. */
        for (size_t pi = 1; pi < prime_count; ++pi) { /* skip prime 2 at pi=0 */
            if (used_flag[pi]) continue;
            const seq_t cand = (seq_t)prime_list[pi];
            const seq_t diff = (cur_val >= cand) ? (cur_val - cand) : (cand - cur_val);
            if (diff % divisor == 0) {
                next_val = cand;
                used_flag[pi] = true;
                break;
            }
        }

        if (next_val == 0) {        /* no further term can be found */
            break;
        }

        cache_seq[cache_len++] = next_val;
    }
}

/* Public interface required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n=0 */

    ensure_up_to(n);
    return (n <= (idx_t)cache_len) ? cache_seq[n - 1] : (seq_t)0;
}
