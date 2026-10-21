/*
 * A091755  Sixth column (m=7) of array A091752 ((-1,2)Stirling2) divided by
 *          -12.
 * https://oeis.org/A091755
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-3)*A052502(n-2)/160 for n≥4 where A052502(k) =
 *            (3k)!/(3^k * k!)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build a prime list up to a given limit */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_cache = NULL;   /* array of primes */
static size_t     prime_cnt   = 0;     /* number of primes stored */
static idx_t      prime_max   = 0;     /* largest prime generated */

/* Ensure that all primes ≤ limit are stored in prime_cache */
static void ensure_primes_upto(idx_t limit) {
    if (limit <= prime_max)               /* nothing to do */
        return;

    /* free old cache if any */
    if (prime_cache != NULL) {
        free(prime_cache);
        prime_cache = NULL;
        prime_cnt   = 0;
    }

    /* build new prime list */
    build_primes_upto(limit, &prime_cache, &prime_cnt);
    prime_max = limit;
}

/* Return the nth term of sequence A091755 (n≥4) */
seq_t a(idx_t n) {
    if (n < 4)   /* value is undefined for n<4 */
        return 0;

    idx_t k    = n - 2;          /* we need A052502(k) */
    idx_t limit = 3 * k;          /* highest prime we will need */

    /* make sure we have primes up to 3k */
    ensure_primes_upto(limit);

    /* Compute A052502(k) = (3k)! / (3^k * k!) via prime exponents */
    seq_t R = 1;

    for (size_t pi = 0; pi < prime_cnt && prime_cache[pi] <= limit; ++pi) {
        uint64_t p = prime_cache[pi];

        idx_t exp_num = 0;   /* exponent of p in (3k)!   */
        idx_t exp_den = 0;   /* exponent of p in k! + 3^k */

        /* exponent of p in (3k)! */
        for (idx_t pw = p; pw <= limit; pw *= p)
            exp_num += limit / pw;

        /* exponent of p in k!   */
        for (idx_t pw = p; pw <= k; pw *= p)
            exp_den += k / pw;

        /* subtract the 3^k factor if p==3 */
        if (p == 3)
            exp_den += k;

        idx_t exp = exp_num - exp_den;
        if (exp == 0)
            continue;

        /* multiply R by p^exp; loop exp times */
        for (idx_t e = 0; e < exp; ++e)
            R *= (seq_t)p;
    }

    /* a(n) = n*(n-3)*R / 160  (R already holds A052502(k)) */
    if (sizeof(seq_t) <= 8) {              /* 64‑bit value: use 128‑bit temp */
        unsigned __int128 tmp = (unsigned __int128)R
                               * (unsigned __int128)n
                               * (unsigned __int128)(n - 3);
        tmp /= 160;
        return (seq_t)tmp;
    } else {                               /* arbitrary‑precision case */
        R *= (seq_t)n;
        R *= (seq_t)(n - 3);
        R /= 160;
        return R;
    }
}
