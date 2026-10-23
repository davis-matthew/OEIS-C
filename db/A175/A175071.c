/*
 * A175071  Natural numbers m with result 1 under iterations of {r mod (max
 *          prime p < r)} starting at r = m.
 * https://oeis.org/A175071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A175071:
 *  a(n) is the n-th natural number m that eventually becomes 1
 *  under the iteration r := r mod (largest prime < r), starting from r = m.
 *  The implementation generates terms by a search and caches them.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- primes up to a fixed limit ---------- */

static const uint64_t PRIME_LIMIT = 2000000ULL; /* big enough for n ≤ 10000 */
static uint32_t *prime_list = NULL;
static size_t   prime_cnt   = 0;
static bool     primes_inited = false;

/* Sieve of Eratosthenes up to PRIME_LIMIT */
static void init_primes(void)
{
    if (primes_inited) return;

    bool *mark = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!mark) exit(1);
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i) mark[i] = true;

    for (uint64_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (mark[i]) {
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i) mark[j] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (mark[i]) ++cnt;

    prime_list = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) exit(1);
    size_t idx = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (mark[i]) prime_list[idx++] = (uint32_t)i;

    prime_cnt = cnt;
    primes_inited = true;
    free(mark);
}

/* Binary search: largest prime index ≤ v (v ≥ 2) */
static inline size_t prime_leq_idx(uint64_t v)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= v) lo = mid + 1;
        else hi = mid;
    }
    return lo - 1;   /* guaranteed v ≥ 2, so lo > 0 */
}

/* --------- test whether a number reaches 1 ---------- */
static inline bool reaches_one(uint64_t m)
{
    if (m == 1) return true;
    if (m == 2) return false;

    uint64_t r = m;
    while (r > 2) {
        uint64_t p = prime_list[prime_leq_idx(r - 1)];
        r = r % p;
    }
    return r == 1;
}

/* ---------- sequence cache ---------- */

static seq_t *cache = NULL;
static idx_t  cache_len   = 0;
static idx_t  cache_cap   = 0;
static idx_t  next_candidate = 1;

/* Ensure cache has capacity for needed elements */
static void ensure_cap(idx_t needed)
{
    if (cache_cap >= needed) return;
    idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
    while (new_cap < needed) new_cap <<= 1;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Populate cache until at least n terms are stored */
static void grow_sequence(idx_t n)
{
    init_primes();
    while (cache_len < n) {
        if (reaches_one((uint64_t)next_candidate)) {
            ensure_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* ---------- public entry point ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* out of range, undefined but safe */
    grow_sequence(n);
    return cache[n - 1];
}
