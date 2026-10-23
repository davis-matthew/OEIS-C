/*
 * A129563  Primes not in a certain recursively defined set of primes.
 * https://oeis.org/A129563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: compute the complement of the recursively defined set M
   by sieving up to a dynamic limit and testing each prime against
   the defining rule. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global state ----------------------------------------------------- */

static bool   *g_is_prime = NULL;   /* primality flags               */
static idx_t  *g_spf      = NULL;   /* smallest prime factor         */
static unsigned char *g_inM = NULL; /* membership in M               */
static uint64_t *g_comp    = NULL;   /* complement primes (store as uint64_t) */
static size_t   g_comp_cap = 0;
static size_t   g_comp_len = 0;
static idx_t    g_limit    = 0;      /* current sieve limit           */

/* ---------------------------------------------------------------- */

static void free_state(void)
{
    free(g_is_prime);
    free(g_spf);
    free(g_inM);
    free(g_comp);
    g_is_prime = NULL;
    g_spf = NULL;
    g_inM = NULL;
    g_comp = NULL;
    g_comp_cap = 0;
    g_comp_len = 0;
    g_limit = 0;
}

/* Build sieve, spf table and M up to “limit”. */
static void build_upto(idx_t limit)
{
    free_state();                   /* clean previous data */
    size_t lim = (size_t)limit;

    g_is_prime = (bool *)calloc(lim + 1, sizeof(bool));
    g_spf      = (idx_t *)calloc(lim + 1, sizeof(idx_t));
    g_inM      = (unsigned char *)calloc(lim + 1, sizeof(unsigned char));
    if (!g_is_prime || !g_spf || !g_inM) exit(1);

    /* initialise primes and spf */
    for (size_t i = 2; i <= lim; ++i) {
        g_is_prime[i] = true;
        g_spf[i]      = (idx_t)i;
    }

    for (size_t i = 2; (size_t)i * i <= lim; ++i) {
        if (!g_is_prime[i]) continue;
        for (size_t j = i * i; j <= lim; j += i) {
            if (g_is_prime[j]) g_is_prime[j] = false;
            if (g_spf[j] == (idx_t)j) g_spf[j] = (idx_t)i;
        }
    }

    /* initial members of M */
    if (limit >= 2) g_inM[2] = 1;
    if (limit >= 3) g_inM[3] = 1;

    /* allocate complement storage */
    g_comp_cap = 256;
    g_comp = (uint64_t *)malloc(g_comp_cap * sizeof(uint64_t));
    if (!g_comp) exit(1);
    g_comp_len = 0;

    /* iterate over primes */
    for (idx_t p = 2; p <= limit; ++p) {
        if (!g_is_prime[p]) continue;
        if (p == 2 || p == 3) continue;   /* already in M */

        idx_t y = p - 1;
        unsigned a = 0, b = 0;

        /* factor out powers of 2 */
        while (((y & 1ULL) == 0) && a <= 41) {
            y >>= 1;
            ++a;
        }
        if (a > 41) continue;

        /* factor out powers of 3 */
        while ((y % 3ULL) == 0 && b <= 46) {
            y /= 3;
            ++b;
        }
        if (b > 46) continue;

        bool ok = true;
        idx_t rem = y;
        while (rem > 1 && ok) {
            idx_t f = g_spf[rem];
            if (f == 0) { ok = false; break; }

            unsigned cnt = 0;
            while (rem % f == 0) {
                rem /= f;
                ++cnt;
            }
            if (cnt != 1) { ok = false; break; }
            if (g_inM[f] == 0) { ok = false; break; }
        }

        if (ok) {
            g_inM[p] = 1;          /* p belongs to M */
        } else {
            /* store as complement prime */
            if (g_comp_len == g_comp_cap) {
                g_comp_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(g_comp,
                                                    g_comp_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                g_comp = tmp;
            }
            g_comp[g_comp_len++] = (uint64_t)p;
        }
    }

    g_limit = limit;
}

/* Public function ------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* guard against offset zero */

    /* Ensure we have enough complement primes.  Double the limit
       until the list is long enough. */
    if (g_comp_len < (size_t)n) {
        idx_t limit = g_limit ? g_limit : 200ULL;
        while (g_comp_len < (size_t)n) {
            limit <<= 1;          /* grow limit exponentially */
            build_upto(limit);
        }
    }

    return (seq_t)g_comp[n - 1];
}
