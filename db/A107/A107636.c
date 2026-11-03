/*
 * A107636  Lesser prime in pair prime(k) +/- k for some k.
 * https://oeis.org/A107636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(A064403(n)) - A064403(n)
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   is_prime_seq, is_prime_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
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
    if (n == 0)              /* undefined for offset 0 – harmless */
        return 0;

    idx_t k = 1;              /* candidate index in the prime list */
    idx_t found = 0;          /* number of qualifying indices seen */

    while (found < n)
    {
        seq_t pk = get_prime((uint64_t)k);      /* p_k */
        seq_t sk = (seq_t)k;                    /* k as seq_t */
        seq_t low  = pk - sk;                   /* p_k - k */
        seq_t high = pk + sk;                   /* p_k + k */

        if (is_prime_seq(low) && is_prime_seq(high))
        {
            found++;
            if (found == n)
                return low;    /* a(n) = p_k - k */
        }

        k++;
    }

    /* unreachable, but keeps the compiler happy */
    return 0;
}
