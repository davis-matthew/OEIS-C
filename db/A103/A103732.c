/*
 * A103732  Negative of column k=5 sequence of array A103728.
 * https://oeis.org/A103732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   is_prime_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A103732
   Formula: a(n) = ((p(n+4)-1)*C(p(n+4)-1,5) - 1)/p(n+4) ,
   where p(k) is the k‑th prime (A000040).  This equals the negative
   of the column‑k=5 entry of array A103728. */

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

/* Compute the binomial coefficient C(t,5) for a seq_t t using
   the multiplicative formula to keep intermediate values small. */
static seq_t
binom5(seq_t t)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= 5; ++i) {
        res *= t - (seq_t)(i - 1);
        res /= (seq_t)i;
    }
    return res;
}

/* Return the n-th term of sequence A103732. */
seq_t
a(idx_t n)
{
    /* Index of the prime we need is n+4 */
    idx_t prime_index = n + 4;

    /* p = p(n+4) */
    seq_t p = get_prime(prime_index);

    /* t = p - 1 */
    seq_t t = p - 1;

    /* bin = C(t,5) */
    seq_t bin = binom5(t);

    /* numerator = t * bin - 1 */
    seq_t num = t * bin;
    num -= 1;

    /* result = num / p  (exact division) */
    seq_t res = num / p;

    return res;
}
