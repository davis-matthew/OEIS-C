/*
 * A240474  Distance from prime(n) to the closest larger squarefree number.
 * https://oeis.org/A240474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A112926(n) - A000040(n) Distance from prime(n) to the
 *            closest larger squarefree number
 * Range:     u8 n <= 55, u16 n <= 8190, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9774 (2026-11-01)
 * Helpers:   build_primes_upto, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_squarefree.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* 0 is unused (offset = 1), returning 0 for safety */
    if (n == 0)
        return 0;

    /* static cache of primes (uint64_t is large enough for the required range) */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;

    /* load primes up to at least the nth prime if not already available */
    if ((size_t)n > prime_cnt) {
        idx_t limit = _prime_limit_for_idx(n);   /* bound for the nth prime */
        uint64_t *new_primes;
        size_t new_cnt;

        build_primes_upto(limit, &new_primes, &new_cnt);

        /* free previous cache if any */
        free(primes);
        primes = new_primes;
        prime_cnt = new_cnt;
    }

    /* prime indexed from 0 in array but OEIS index starts at 1 */
    uint64_t p_val = primes[n - 1];
    seq_t p = (seq_t)p_val;          /* convert to seq_t (works for all seq_t) */

    /* search for the first square‑free integer > p */
    seq_t m = p + 1;
    while (!is_squarefree(m))
        ++m;

    return m - p;   /* distance to the nearest larger square‑free number */
}
