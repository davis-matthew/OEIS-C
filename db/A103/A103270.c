/*
 * A103270  a(n) = (prime(n)+prime(n+k)) mod 4, where k =
 *          (prime(n+1)-prime(n))/2.
 * https://oeis.org/A103270
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)+prime(n+k)) mod 4, where k =
 *            (prime(n+1)-prime(n))/2
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9006 (2026-11-01)
 * Helpers:   is_prime_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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
    if (n < 2)               /* offset is 2; behaviour below is irrelevant */
        return (seq_t)0;

    /* prime(n), prime(n+1), prime(n+k) */
    seq_t p1 = get_prime(n);          /* prime(n)   */
    seq_t p2 = get_prime(n + 1);      /* prime(n+1) */

    /* k = (prime(n+1)-prime(n))/2 */
    seq_t diff_half = (p2 - p1) / (seq_t)2;
    idx_t k = (idx_t)diff_half;       /* k is always an integer */

    seq_t p3 = get_prime(n + k);      /* prime(n+k) */

    /* (prime(n)+prime(n+k)) mod 4 */
    return (p1 + p3) % (seq_t)4;
}
