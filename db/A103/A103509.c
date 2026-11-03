/*
 * A103509  a(n) is the least j such that 2n+1 = 2*A000040(k) + A000040(j) for
 *          some k > 1, or 0 if no such j exists.
 * https://oeis.org/A103509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least j such that 2*n+1 = 2*A000040(k) + A000040(j) for
 *            some k > 1, or 0 if no such j exists
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 8830 (2026-11-03)
 * Helpers:   is_prime_trial, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_trial.h"
#include "../utils/is_prime_u64.h"

/*--- dynamic array helpers for primes --------------------------------------*/
static void ensure_primes_cap(size_t *cap, uint64_t **arr, size_t min_cap) {
    if (*cap < min_cap) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t get_prime(uint64_t idx) {
    static uint64_t *prime_vec = NULL;
    static size_t prime_cap = 0;
    static size_t prime_sz  = 0;
    static uint64_t candidate = 3;      /* next odd to test */

    if (!prime_vec) { /* first call: initialise */
        ensure_primes_cap(&prime_cap, &prime_vec, 16);
        prime_vec[0] = 2;
        prime_sz = 1;
    }

    while (prime_sz < idx) {
        /* find next prime */
        while (!is_prime_trial(candidate, prime_vec, prime_sz))
            candidate += 2;

        /* add found prime */
        if (prime_sz == prime_cap)
            ensure_primes_cap(&prime_cap, &prime_vec, prime_cap << 1);

        prime_vec[prime_sz++] = candidate;
        candidate += 2;
    }
    return (seq_t)prime_vec[idx - 1];
}

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1; a(1), a(2), a(3) are 0 by definition. */
    if (n < 4) {
        return 0;
    }

    /* target = 2*n + 1  as a 64‑bit integer (fits for all n required). */
    uint64_t target = (uint64_t)(2 * n) + 1;

    /* The largest prime pj we can use is when k=2 (prime(k)=3). */
    uint64_t max_pj = (target > 3 ? target - 3 : 0);

    for (uint64_t j = 1; ; ++j) {
        /* pj = A000040(j). */
        seq_t pj_seq = get_prime(j);
        uint64_t pj = (uint64_t)pj_seq;

        if (pj > max_pj) {
            break;               /* No further j can satisfy the equation. */
        }

        uint64_t d = target - pj;    /* d = 2*prime(k).   */
        if ((d & 1ULL) != 0) {       /* d must be even.   */
            continue;
        }

        uint64_t q = d >> 1;        /* Candidate prime(k). */
        if (q >= 3 && is_prime_u64(q)) {
            return (seq_t)j;        /* Smallest such j found. */
        }
    }

    return 0;                       /* No suitable j exists.   */
}
