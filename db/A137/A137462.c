/*
 * A137462  a(n) + a(n-1) = n-th semiprime.
 * https://oeis.org/A137462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(n)+a(n-1)=n-th semiprime (A001358)
 * Range:     u8 n <= 134, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- Static storage ---------- */
static seq_t *a_cache = NULL;      /* a(0)…a(a_len-1) */
static size_t a_cap   = 0;         /* allocated capacity of a_cache   */
static size_t a_len   = 0;         /* number of computed entries   */

static uint64_t *semip_cache = NULL;   /* n‑th semiprime, 1‑based index   */
static size_t   semip_len   = 0;         /* number of semiprimes computed   */
static uint64_t semip_limit = 0;        /* current sieve bound             */

static uint32_t *spf = NULL;       /* smallest prime factor, size = limit+1 */
static size_t   spf_sz = 0;         /* current size                        */
static uint32_t *prime_list = NULL; /* list of primes during sieving       */
static size_t   prime_cnt  = 0;      /* number of primes                     */
static size_t   prime_cap  = 0;      /* allocated capacity of prime_list     */

/* ---------- Sieve of smallest prime factors ---------- */
static void build_spf(uint64_t limit)
{
    /* free old data */
    free(spf);
    free(prime_list);

    spf_sz = limit + 1;
    spf = (uint32_t *)calloc(spf_sz, sizeof(uint32_t));
    if (!spf) return;        /* allocation failure – caller will try again */

    prime_cap = limit / 10 + 10;
    prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_list) { free(spf); spf = NULL; return; }

    prime_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = (uint32_t)i;
            if (prime_cnt < prime_cap) prime_list[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_list[j];
            uint64_t ip = i * (uint64_t)p;
            if (p > spf[i] || ip > limit) break;
            spf[ip] = p;
        }
    }
}

/* ---------- Generate semiprimes up to current limit ---------- */
static void generate_semiprimes(void)
{
    size_t count = 0;

    for (uint64_t i = 4; i <= semip_limit; ++i) {
        uint32_t p = spf[i];
        if (p == i) continue;                  /* prime */
        uint64_t q = i / p;
        if (q == p || spf[q] == (uint32_t)q) ++count;
    }

    /* free old cache */
    free(semip_cache);
    semip_cache = (uint64_t *)malloc((count + 1) * sizeof(uint64_t));
    if (!semip_cache) { semip_len = 0; return; }

    size_t idx = 1;
    for (uint64_t i = 4; i <= semip_limit; ++i) {
        uint32_t p = spf[i];
        if (p == i) continue;
        uint64_t q = i / p;
        if (q == p || spf[q] == (uint32_t)q) {
            semip_cache[idx++] = i;
        }
    }
    semip_len = count;   /* usable indices 1 … semip_len */
}

/* ---------- Ensure we have at least `needed` semiprimes ---------- */
static void ensure_semiprimes(size_t needed)
{
    if (semip_len >= needed) return;

    uint64_t limit = semip_limit ? semip_limit * 2 : 1000000ULL; /* start at 1 M */
    while (true) {
        if (limit < 4) limit = 4;                /* minimum meaningful value */
        build_spf(limit);
        semip_limit = limit;
        generate_semiprimes();
        if (semip_len >= needed) break;
        limit *= 2;   /* grow and retry */
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Initialise a_cache on first use */
    if (a_len == 0) {
        a_cap = 16;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (!a_cache) return (seq_t)0;     /* allocation failure – return 0 */
        a_cache[0] = (seq_t)1;
        a_len = 1;
    }

    /* If the value is already computed, return it */
    if ((idx_t)a_len > n) return a_cache[(size_t)n];

    /* Make sure we can address semiprime(n) */
    ensure_semiprimes((size_t)n);

    /* Grow a_cache if necessary */
    if ((size_t)a_len <= (size_t)n) {
        size_t new_cap = a_cap ? a_cap : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        a_cache = tmp;
        a_cap = new_cap;
    }

    /* Compute a(k) for k = a_len … n */
    for (size_t k = a_len; k <= (size_t)n; ++k) {
        uint64_t semip_val = semip_cache[k];                 /* semiprime(k) */
        unsigned long long prev = (unsigned long long)a_cache[k - 1];
        unsigned long long val  = semip_val - prev;          /* a(k) = semip‑a(k-1) */
        a_cache[k] = (seq_t)val;
    }
    a_len = (size_t)n + 1;

    return a_cache[(size_t)n];
}
