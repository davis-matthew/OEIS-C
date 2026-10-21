/*
 * A066311  Numbers whose distinct prime factors are consecutive.
 * https://oeis.org/A066311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers whose distinct prime factors are consecutive primes
 * Range:     u8 n <= 103, u16 n <= 7125, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term (1‑based) of OEIS A066311.
   Terms are generated lazily and cached in a static array.  The
   implementation only uses 64‑bit integer arithmetic for
   factorisation and a consecutive‑prime test.  */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/seq_realloc.h"

static seq_t *series = NULL;      /* cached terms */
static idx_t series_len = 0;      /* number of cached terms */
static idx_t series_cap = 0;      /* capacity of series */
static uint64_t next_candidate = 2;   /* next integer to test */

/* helper: add a term to the cache, expanding the array if needed */
static void add_term(seq_t val)
{
    if (series_len == series_cap) {
        series_cap = series_cap ? series_cap * 2 : 2;
        series = seq_realloc(series, series_len, series_cap);
    }
    series[series_len++] = val;
}

seq_t a(idx_t n)
{
    /* ensure we have at least n terms cached */
    while (series_len < n) {
        /* find the next term */
        while (1) {
            size_t cnt;
            uint64_t primes[20], exps[20];
            factor_u64(next_candidate, primes, exps, &cnt);

            bool ok = true;
            for (size_t i = 1; i < cnt; ++i) {
                if (next_prime(primes[i - 1]) != primes[i]) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                add_term((seq_t)next_candidate);
                break;
            }
            ++next_candidate;
        }
        /* advance to the next integer to test */
        ++next_candidate;
    }
    return series[n - 1];
}
