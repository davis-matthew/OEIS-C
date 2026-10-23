/*
 * A153478  Sum of first n isolated (or single) primes A007510.
 * https://oeis.org/A153478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 111, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of the first n isolated primes (OEIS A153478). */
/*  a(n) = p1 + p2 + … + pn   where pi are isolated primes.
    A prime p is isolated if p−2 and p+2 are both composite. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/is_prime.h"           /* is_prime */

/* Cached cumulative sums of isolated primes.
   cum[0] = 0, cum[i] = sum of first i isolated primes. */
static seq_t *cum = NULL;
static size_t cum_len = 0;            /* stored length = number_of_isolated + 1 */

static void
ensure_cached(idx_t n)
{
    /* If we already have enough data, nothing to do. */
    if (cum_len > 0 && (idx_t)(cum_len - 1) >= n)
        return;

    /* Estimate a limit that should contain at least n isolated primes.
       The bound is crude but sufficient for n up to several 10⁴. */
    idx_t limit = (idx_t)(n * 25);     /* rough over‑approximation */
    if (limit < 100)
        limit = 100;

    /* Try increasing the limit until we have enough isolated primes. */
    size_t iso_needed = (size_t)n;
    size_t iso_cnt = 0;
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;

    while (1) {
        /* Generate all primes up to the current limit. */
        build_primes_upto(limit, &primes, &prime_cnt);

        /* Count isolated primes in this range. */
        iso_cnt = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (!is_prime(p - 2) && !is_prime(p + 2))
                ++iso_cnt;
        }

        if (iso_cnt >= iso_needed)
            break;                  /* we have enough isolated primes */

        /* Not enough – enlarge the limit and try again. */
        free(primes);
        limit *= 2;
    }

    /* Build the cumulative sums array. */
    seq_t *new_cum = (seq_t *)malloc((iso_cnt + 1) * sizeof(seq_t));
    if (!new_cum) abort();           /* unlikely but safe */

    new_cum[0] = (seq_t)0;
    seq_t sum = (seq_t)0;
    size_t idx = 0;                   /* number of isolated primes processed */

    for (size_t i = 0; i < prime_cnt && idx < iso_cnt; ++i) {
        uint64_t p = primes[i];
        if (!is_prime(p - 2) && !is_prime(p + 2)) {
            sum += (seq_t)p;
            new_cum[++idx] = sum;
        }
    }

    /* Replace the old cache. */
    free(cum);
    cum = new_cum;
    cum_len = iso_cnt + 1;

    /* Free the prime list (we no longer need it). */
    free(primes);
}

seq_t a(idx_t n)
{
    ensure_cached(n);
    return cum[n];
}
