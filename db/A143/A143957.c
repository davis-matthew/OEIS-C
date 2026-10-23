/*
 * A143957  An integer >= 2 is included if {the difference between the largest
 *          and smallest primes dividing n} divides n+1.
 * https://oeis.org/A143957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer m ≥ 2 with (largest prime divisor of m –
 *            smallest prime divisor of m) dividing (m + 1)
 * Range:     u8 n <= 42, u16 n <= 881, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Global state shared by all calls of a() */
static uint64_t *g_spf          = NULL;   /* smallest prime factor array   */
static uint64_t g_spf_limit     = 0;      /* highest index currently factored */
static seq_t   *g_seq           = NULL;   /* sequence store */
static size_t  g_seq_size       = 0;      /* number of sequence terms stored */
static size_t  g_seq_capacity   = 0;      /* allocated capacity for g_seq   */
static uint64_t g_next_candidate = 2;     /* next number to test            */
static int      g_initialized   = 0;      /* flag to indicate first call    */

/* Re‑sieve spf array up to new_limit (inclusive). */
static void recompute_spf(uint64_t new_limit)
{
    uint64_t *new_spf = (uint64_t *)realloc(g_spf, (new_limit + 1) * sizeof(uint64_t));
    if (!new_spf) exit(1);
    g_spf = new_spf;

    for (uint64_t i = g_spf_limit + 1; i <= new_limit; ++i)
        g_spf[i] = i;

    for (uint64_t i = 2; i * i <= new_limit; ++i) {
        if (g_spf[i] == i) {                     /* i is prime */
            for (uint64_t j = i * i; j <= new_limit; j += i)
                if (g_spf[j] == j)
                    g_spf[j] = i;
        }
    }
    g_spf_limit = new_limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Initialise static data on first call */
    if (!g_initialized) {
        /* Start with a sieve large enough for n up to about 10 000 */
        g_spf_limit = 1000000;
        g_spf = (uint64_t *)calloc(g_spf_limit + 1, sizeof(uint64_t));
        if (!g_spf) exit(1);
        for (uint64_t i = 2; i <= g_spf_limit; ++i)
            g_spf[i] = i;
        for (uint64_t i = 2; i * i <= g_spf_limit; ++i)
            if (g_spf[i] == i)
                for (uint64_t j = i * i; j <= g_spf_limit; j += i)
                    if (g_spf[j] == j)
                        g_spf[j] = i;

        g_seq_capacity = 16;
        g_seq = (seq_t *)malloc(g_seq_capacity * sizeof(seq_t));
        if (!g_seq) exit(1);
        g_initialized = 1;
    }

    /* Extend the sequence until it contains at least n terms */
    while (g_seq_size < n) {
        /* Make sure spf covers the candidate */
        if (g_next_candidate > g_spf_limit) {
            uint64_t new_lim = g_spf_limit * 2;
            if (new_lim < g_next_candidate * 2)
                new_lim = g_next_candidate * 2;
            recompute_spf(new_lim);
        }

        uint64_t m = g_next_candidate;
        uint64_t tmp = m;
        uint64_t min_pf = g_spf[m];
        uint64_t max_pf = min_pf;

        while (tmp > 1) {
            uint64_t p = g_spf[tmp];
            if (p < min_pf) min_pf = p;
            if (p > max_pf) max_pf = p;
            tmp /= p;
        }

        uint64_t diff = max_pf - min_pf;
        if (diff != 0 && (uint64_t)((m + 1) % diff) == 0) {
            if (g_seq_size >= g_seq_capacity) {
                g_seq_capacity *= 2;
                seq_t *tmp_seq = (seq_t *)realloc(g_seq, g_seq_capacity * sizeof(seq_t));
                if (!tmp_seq) exit(1);
                g_seq = tmp_seq;
            }
            g_seq[g_seq_size++] = (seq_t)m;
        }

        ++g_next_candidate;
    }

    return g_seq[n - 1];
}
