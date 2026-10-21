/*
 * A079132  Primes such that iterated sum-of-digits (A038194) is even.
 * https://oeis.org/A079132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   iterated_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A079132: primes p such that the iterated sum‑of‑digits
 * of p is even (A038194 is even).  The routine builds a cache of
 * primes and of primes satisfying the condition.  Subsequent calls
 * extend the cache from where it left off; therefore the function
 * satisfies arbitrary calling order and runs in about O(n) time.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/iterated_even.h"

/*--- helper: ensure that the allocated buffer has at least min_cap elements ---*/
static void ensure_cap(void **ptr, size_t *capp, size_t min_cap, size_t elem_sz)
{
    if (*capp < min_cap) {
        size_t new_cap = (*capp) ? (*capp) * 2 : 4;
        if (new_cap < min_cap) new_cap = min_cap;
        *ptr = realloc(*ptr, new_cap * elem_sz);
        *capp = new_cap;
    }
}

/*--- main sequence function ---*/
seq_t a(idx_t n)
{
    /* cache of primes that satisfy the parity condition */
    static seq_t *seq  = NULL;
    static size_t seq_cap = 0;
    static idx_t seq_len  = 0;

    /* list of all primes discovered so far (used for trial division) */
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static idx_t primes_cnt = 0;

    /* enlarge result cache until we have the desired n-th term */
    while (seq_len < n) {
        /* find next prime candidate */
        seq_t candidate;
        if (primes_cnt == 0) {
            candidate = 2;
        } else {
            candidate = primes[primes_cnt - 1] + 1;
            if (candidate % 2 == 0) candidate++;   /* skip even numbers after 2 */
        }

        /* trial‑divide candidate using already known primes */
        while (1) {
            bool isprime = true;
            for (idx_t i = 0; i < primes_cnt; ++i) {
                seq_t p = primes[i];
                seq_t div = candidate / p;
                if (p > div) break;          /* p*p > candidate, stop */
                if (candidate % p == 0) {
                    isprime = false;
                    break;
                }
            }
            if (isprime) break;
            candidate += (candidate == 2) ? 1 : 2;   /* keep odd if >2 */
        }

        /* store the new prime */
        ensure_cap((void **)&primes, &primes_cap, primes_cnt + 1, sizeof(seq_t));
        primes[primes_cnt++] = candidate;

        /* if it satisfies the parity test, keep it in the result cache */
        if (iterated_even(candidate)) {
            ensure_cap((void **)&seq, &seq_cap, seq_len + 1, sizeof(seq_t));
            seq[seq_len++] = candidate;
        }
    }

    return seq[n - 1];
}
