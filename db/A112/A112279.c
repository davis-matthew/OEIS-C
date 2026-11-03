/*
 * A112279  a(1)=1; a(n)=prime(mod(a(n-1),100)).
 * https://oeis.org/A112279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less, changed
 * Formula:   a(1)=1; a(n)=prime(mod(a(n-1),100))
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   is_prime_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_trial.h"

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
    /* This sequence becomes periodic with period 5 after a(3).
       Compute the first 5 periodic values once and reuse them
       for any n.  The periodic block is {5,11,31,127,103}. */
    static seq_t period[5];
    static bool initialized = false;

    if (!initialized) {
        seq_t prev = (seq_t)1;          /* a(1) */
        seq_t curr = (seq_t)0;          /* will hold a(i) */
        /* Compute a(2) through a(8) to extract the period. */
        for (idx_t i = 2; i <= 8; ++i) {
            seq_t idx = prev % (seq_t)100;
            curr  = get_prime((uint64_t)idx);
            if (i >= 4) period[i - 4] = curr;   /* store a(4) .. a(8) */
            prev = curr;
        }
        initialized = true;
    }

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    idx_t pos = (n - 4) % 5;       /* index into the period */
    return period[pos];
}
