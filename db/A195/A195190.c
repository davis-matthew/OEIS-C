/*
 * A195190  Primes p such that there is only one prime number between
 *          semiprime(p) and semiprime(p+1).
 * https://oeis.org/A195190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1157, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A195190: primes p such that there is exactly one prime between semiprime(p) and semiprime(p+1) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool          g_initialized = false;
static uint32_t     *g_spf          = NULL;   /* smallest prime factor for numbers up to LIMIT */
static uint32_t     *g_prime        = NULL;   /* list of primes up to LIMIT            */
static size_t        g_prime_cnt    = 0;
static size_t        g_prime_cap    = 0;
static seq_t        *g_semiprimes   = NULL;   /* semiprime numbers (1‑based)            */
static size_t        g_semip_len    = 0;
static size_t        g_semip_cap    = 0;
static seq_t        *g_answers      = NULL;   /* list of primes satisfying the condition */
static size_t        g_ans_len      = 0;
static size_t        g_ans_cap      = 0;

/* grow a dynamic array of uint32_t */
static void grow_uint32(uint32_t **arr, size_t *len, size_t *cap, size_t add) {
    if (*len + add > *cap) {
        *cap = (*cap == 0) ? 16 : (*cap * 2);
        *arr = (uint32_t*)realloc(*arr, *cap * sizeof(uint32_t));
    }
}

/* grow a dynamic array of seq_t */
static void grow_seq(seq_t **arr, size_t *len, size_t *cap, size_t add) {
    if (*len + add > *cap) {
        *cap = (*cap == 0) ? 16 : (*cap * 2);
        *arr = (seq_t*)realloc(*arr, *cap * sizeof(seq_t));
    }
}

/* one‑time initialization: sieve, semiprime list, answer list */
static void init_once(void) {
    if (g_initialized) return;
    g_initialized = true;

    /* 1. linear sieve for SPF and prime list */
    const size_t LIMIT = 10000000;                 /* maximum number to sieve */
    g_spf        = (uint32_t*)calloc(LIMIT + 1, sizeof(uint32_t));
    g_prime_cap  = 800000;                         /* enough for primes up to LIMIT */
    g_prime      = (uint32_t*)malloc(g_prime_cap * sizeof(uint32_t));

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (g_spf[i] == 0) {                      /* i is prime */
            g_spf[i] = i;
            grow_uint32(&g_prime, &g_prime_cnt, &g_prime_cap, 1);
            g_prime[g_prime_cnt++] = i;
        }
        for (size_t j = 0; j < g_prime_cnt; ++j) {
            uint32_t p = g_prime[j];
            if (p > g_spf[i] || (uint64_t)i * p > LIMIT) break;
            g_spf[i * p] = p;
        }
    }

    /* 2. build semiprime list (numbers with exactly two prime factors) */
    g_semip_cap   = 2000000;                      /* initial capacity */
    g_semiprimes  = (seq_t*)malloc(g_semip_cap * sizeof(seq_t));

    for (uint32_t n = 4; n <= LIMIT; ++n) {
        uint32_t p = g_spf[n];
        uint32_t q = n / p;
        if (g_spf[q] == q) {                      /* both p and q are prime */
            grow_seq(&g_semiprimes, &g_semip_len, &g_semip_cap, 1);
            g_semiprimes[g_semip_len++] = (seq_t)n;
        }
    }

    /* 3. build answer list: primes p where exactly one prime lies between
       semiprime(p) and semiprime(p+1) */
    g_ans_cap   = 100000;                          /* initial capacity */
    g_answers   = (seq_t*)malloc(g_ans_cap * sizeof(seq_t));

    for (size_t idx = 0; idx < g_prime_cnt; ++idx) {
        uint32_t p = g_prime[idx];
        if ((size_t)p >= g_semip_len) break;      /* no semiprime(p+1) available */

        seq_t low  = g_semiprimes[p - 1];          /* semiprime(p)   */
        seq_t high = g_semiprimes[p];              /* semiprime(p+1) */

        /* count primes in (low, high) */
        uint32_t count = 0;
        for (uint32_t x = (uint32_t)low + 1; x < (uint32_t)high; ++x) {
            if (g_spf[x] == x) ++count;           /* x is prime */
        }

        if (count == 1) {
            grow_seq(&g_answers, &g_ans_len, &g_ans_cap, 1);
            g_answers[g_ans_len++] = (seq_t)p;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n<1 */
    init_once();
    if ((size_t)(n - 1) >= g_ans_len) return 0;     /* out of range */
    return g_answers[n - 1];
}
