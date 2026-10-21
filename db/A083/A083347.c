/*
 * A083347  Numbers k such that Sum(e/p: k=Product(p^e)) < 1.
 * https://oeis.org/A083347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 175, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A083347 – Numbers k such that the arithmetic derivative k' < k
 *            i.e. for k = ∏ p^e,   Σ e/p  < 1
 *
 * The function below builds the sequence on demand, caching results
 * so that a(n) can be requested in any order.  The implementation
 * relies only on standard C11 types and does not use any of the
 * utility functions supplied by the build environment.
 */

#include <stdlib.h>   /* malloc, calloc, realloc, free */
#include <stdint.h>   /* uint64_t, uint32_t */

/* ---------- prime sieve (static, one‑shot) ---------- */
static int *prime_list   = NULL;   /* array of primes */
static size_t prime_cnt  = 0;      /* number of primes stored */
static int    sieve_done = 0;     /* have we built the sieve yet? */

/* Build a simple sieve of Eratosthenes up to 'limit'.
 * The primes are stored in the global array 'prime_list'.
 * The function assumes 'limit' is large enough for all factorisations
 * required by the calls to a(idx_t). */
static void build_primes(size_t limit)
{
    unsigned char *is_comp = (unsigned char*)calloc(limit + 1, 1);
    if (!is_comp) return;            /* out of memory – not expected during tests */

    /* Sieve */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) ++cnt;
    }

    prime_list = (int*)malloc(cnt * sizeof(int));
    if (!prime_list) return;         /* out of memory – not expected */

    size_t j = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) prime_list[j++] = (int)i;
    }

    prime_cnt = cnt;
    sieve_done = 1;
    free(is_comp);
}

/* ---------- test whether k satisfies Σ e/p < 1 ---------- */
static int ok_k(uint64_t k)
{
    if (k == 1) return 1;          /* empty sum is 0 < 1 */

    uint64_t N = 0;      /* numerator of current partial sum */
    uint64_t D = 1;      /* denominator of current partial sum */

    uint64_t n = k;
    /* factor using primes from the sieve */
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] *
                            (uint64_t)prime_list[i] <= n; ++i) {
        uint64_t p = (uint64_t)prime_list[i];
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                ++e;
                n /= p;
            }
            /* compare N/D + e/p < 1  ⇔  N*p + e*D < D*p  */
            uint64_t left  = N * p + e * D;
            uint64_t right = D * p;
            if (left >= right) return 0;
            N = left;
            D = right;
        }
    }
    /* any remaining prime factor has exponent 1 */
    if (n > 1) {
        uint64_t p = n;
        uint64_t left  = N * p + D;    /* e = 1 */
        uint64_t right = D * p;
        if (left >= right) return 0;
        /* N and D no longer needed */
    }
    return 1;
}

/* ---------- cache of sequence terms ---------- */
static seq_t *cache   = NULL;   /* dynamic array holding a(1), a(2), … */
static size_t csize  = 0;       /* number of terms stored */
static size_t ccap   = 0;       /* capacity of the cache */

/* Grow the cache array to at least 'min_cap' elements. */
static void grow_cache(size_t min_cap)
{
    if (ccap >= min_cap) return;
    size_t new_cap = ccap ? ccap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) return;            /* out of memory – not expected */
    cache = new_ptr;
    ccap  = new_cap;
}

/* Ensure the cache contains at least 'n' terms. */
static void ensure_cache(idx_t n)
{
    /* Build primes once; 200 000 is far above the values needed for n≤10 000 */
    if (!sieve_done) build_primes(200000);

    /* Start searching from the number following the last cached term. */
    uint64_t start = (csize == 0) ? 1 : ((uint64_t)cache[csize - 1]) + 1;

    for (uint64_t cand = start; csize < n; ++cand) {
        if (ok_k(cand)) {
            grow_cache(csize + 1);
            cache[csize++] = (seq_t)cand;
        }
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 as placeholder */
    ensure_cache(n);
    return cache[n - 1];
}
