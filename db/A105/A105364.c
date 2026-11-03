/*
 * A105364  Rearrangement of positive integers according to parity of
 *          (prime(n)-1)/2 (see comments).
 * https://oeis.org/A105364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-01)
 * Helpers:   is_prime_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) from OEIS A105364
 *
 *  The terms are produced by repeatedly selecting the next unused odd
 *  or even integer.  For step n (1‑based) we look at the prime number
 *  prime(n+1).  Let x = (prime(n+1) – 1)/2.  If x is odd the next
 *  unused even number is chosen; if x is even the next unused odd
 *  number is chosen.  The chosen integer is the value of a(n).
 *
 *  The implementation recomputes the sequence from scratch for each
 *  call, so calls may be made in any order.  It uses the helper
 *  function get_prime from "../utils/get_prime.h".
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
    seq_t next_odd  = 1;   /* next unused odd number   */
    seq_t next_even = 2;   /* next unused even number  */
    seq_t res       = 0;   /* placeholder for result   */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t prime = get_prime(i + 1);          /* prime(i+1)   */
        seq_t parity = ((prime - 1) / 2) & (seq_t)1; /* 0 = even, 1 = odd */

        if (parity) {             /* odd -> use next even */
            res = next_even;
            next_even += 2;
        } else {                  /* even -> use next odd */
            res = next_odd;
            next_odd += 2;
        }
    }
    return res;
}
