/*
 * A168433  One half of (n-th sum of primes between successive pairs of twin
 *          primes minus n-th number of primes between successive pairs of
 *          twin primes).
 * https://oeis.org/A168433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A078731(n) - A048614(n)) / 2 A078731(n) is the sum of
 *            primes strictly between the n-th and (n+1)-th twin prime pairs;
 *            A048614(n) is the number of such primes
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

static uint64_t *g_primes      = NULL; /* primes list */
static size_t   g_prime_cnt    = 0;     /* number of primes */
static idx_t   *g_twin_start   = NULL; /* index of first prime in each twin pair */
static size_t   g_twin_cnt     = 0;     /* number of twin pairs */

/* Rough upper bound for the n-th twin prime (not guaranteed, but safe). */
static idx_t estimate_limit_for_n(idx_t n)
{
    if (n < 6) return 15;
    double dn = (double)n;
    double est = dn * (log(dn) + log(log(dn)));
    return (idx_t)(est * 1.5) + 100;
}

/* Build enough twin pairs to satisfy index n.  The lists are kept until
   larger limits are needed.  Safe for repeated calls. */
static void ensure_twin_pairs(idx_t n)
{
    if (g_twin_cnt > n) return;      /* already sufficient */

    idx_t limit = estimate_limit_for_n(n + 1);

    while (1) {
        uint64_t *tmp_primes = NULL;
        size_t tmp_cnt = 0;
        build_primes_upto(limit, &tmp_primes, &tmp_cnt);

        /* Collect twin pair start indices. */
        idx_t *tmp_twin = NULL;
        size_t twin_cap = 0;
        size_t local_cnt = 0;

        for (size_t i = 0; i + 1 < tmp_cnt; ++i) {
            if (tmp_primes[i + 1] == tmp_primes[i] + 2) {
                if (local_cnt == twin_cap) {
                    twin_cap = twin_cap == 0 ? 8 : twin_cap * 2;
                    idx_t *newp = (idx_t *)realloc(tmp_twin, twin_cap * sizeof(idx_t));
                    if (!newp) {                /* allocation failed */
                        free(tmp_primes);
                        free(tmp_twin);
                        return;
                    }
                    tmp_twin = newp;
                }
                tmp_twin[local_cnt++] = (idx_t)i;   /* index of first prime in twin pair */
            }
        }

        if (local_cnt <= n) {
            /* Need more primes; try a larger limit. */
            free(tmp_primes);
            free(tmp_twin);
            limit *= 2;
            continue;
        }

        /* Replace the global data with the new, larger set. */
        free(g_primes);
        free(g_twin_start);
        g_primes      = tmp_primes;
        g_prime_cnt   = tmp_cnt;
        g_twin_start  = tmp_twin;
        g_twin_cnt    = local_cnt;
        break;
    }
}

/* Public function that returns a(n).  It is safe to call in any order. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    ensure_twin_pairs(n);

    idx_t idx      = n - 1;                     /* zero‑based interval index */
    idx_t start1   = g_twin_start[idx];
    idx_t start2   = g_twin_start[idx + 1];

    seq_t sum = (seq_t)0;
    if (start2 > start1 + 2) {
        for (idx_t i = start1 + 2; i < start2; ++i) {
            sum += (seq_t)g_primes[i];
        }
    }

    seq_t cnt = (start2 > start1 + 2) ?
                (seq_t)(start2 - start1 - 2) : (seq_t)0;

    return (sum - cnt) / (seq_t)2;
}
