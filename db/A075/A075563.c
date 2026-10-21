/*
 * A075563  Rearrangement of natural numbers such that n-th partial sum is ==
 *          n (mod p(n)).
 * https://oeis.org/A075563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   optimal rearrangement of 1,2,3,… such that the n‑th partial sum
 *            S(n) satisfies S(n) ≡ n (mod p(n)), where p(n) is the n‑th
 *            prime. The sequence is generated algorithmically by, for every
 *            n, choosing the smallest unused positive integer a(n) that
 *            satisfies the congruence. This reproduces A075563
 * Range:     u8 n <= 16, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>          /* for log() in prime estimation */

/* Global state shared by multiple calls */
static bool primes_inited = false;
static uint64_t *primes = NULL;   /* array of primes */
static size_t primes_cnt = 0;     /* number of primes stored */

static seq_t *cache = NULL;       /* cached values a(1)..a(max_n) */
static size_t cache_cap = 0;      /* capacity allocated for cache */

static bool *used = NULL;         /* bitmap of used integers */
static size_t used_cap = 0;       /* capacity allocated for used */

static idx_t  max_n_calculated = 0; /* highest n already processed */
static seq_t  partial_sum = 0;       /* S(max_n_calculated) */

/* ------------------------------------------------------------------ */
/* Simple sieve to generate all primes up to LIMIT.  LIMIT must be
   at least 200000 to cover the 10000th prime (≈104729).               */
static void init_primes(void)
{
    const idx_t LIMIT = 200000;
    bool *is_comp = (bool*)calloc((size_t)(LIMIT+1), sizeof(bool));
    if (!is_comp) abort();

    uint64_t *prime_buf = NULL;
    size_t prime_cap = 128;
    prime_buf = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    if (!prime_buf) abort();

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt == prime_cap) {
                prime_cap <<= 1;
                prime_buf = (uint64_t*)realloc(prime_buf, prime_cap * sizeof(uint64_t));
                if (!prime_buf) abort();
            }
            prime_buf[primes_cnt++] = (uint64_t)i;
            if ((idx_t)i * i <= LIMIT) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }

    primes = prime_buf;
    free(is_comp);
    primes_inited = true;
}

/* ------------------------------------------------------------------ */
/* Grow the used bitmap to at least 'need' elements. */
static void ensure_used_capacity(size_t need)
{
    if (need <= used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    bool *new_used = (bool*)realloc(used, new_cap * sizeof(bool));
    if (!new_used) abort();
    memset(new_used + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = new_used;
    used_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Grow the cache array to at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* The main function asked for by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but safe */

    /* Ensure primes are ready. */
    if (!primes_inited) init_primes();

    /* If the desired index is already computed, return it. */
    if (n <= max_n_calculated) return cache[n-1];

    /* Compute from max_n_calculated+1 up to n. */
    for (idx_t k = max_n_calculated + 1; k <= n; ++k) {
        uint64_t p = primes[k-1];           /* k‑th prime */
        /* r = (k - partial_sum) mod p  ,  0 <= r < p */
        seq_t k_mod   = (seq_t)k % p;
        seq_t sum_mod = partial_sum % p;
        seq_t r = (k_mod + p - sum_mod) % p;

        /* Candidate: smallest positive integer ≡ r (mod p) */
        size_t candidate = (r == 0) ? (size_t)p : (size_t)r;

        /* Find the first unused such integer. */
        while (true) {
            ensure_used_capacity(candidate + 1);
            if (!used[candidate]) break;
            candidate += (size_t)p;
        }

        /* Record the value. */
        seq_t val = (seq_t)candidate;
        ensure_cache_capacity(k);
        cache[k-1] = val;
        used[candidate] = true;
        partial_sum += val;
        max_n_calculated = k;
    }

    /* Return the requested term. */
    return cache[n-1];
}
