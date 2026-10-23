/*
 * A193339  Indices of record values in A001783.
 * https://oeis.org/A193339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 6544, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, prime_limit_for_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Indices of record values in A001783.
 *
 * Sequence starts 1,3,4,5,7,9 and thereafter coincides with all odd primes.
 * Thus for n ≥ 7 the value is the (n‑2)-th prime.  The first six terms are
 * handled explicitly.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/prime_limit_for_idx.h"
#include "../utils/build_primes.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Handle the first six exceptional terms. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)5;
    if (n == 5) return (seq_t)7;
    if (n == 6) return (seq_t)9;

    /* For n ≥ 7 the term is prime(n‑2). */
    idx_t need = n - 2;          /* 1‑based index of the required prime */

    /* Static array of primes and its size. */
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0; /* number of primes stored */

    /* If we don't have enough primes, enlarge the table. */
    if ((size_t)need > prime_cnt) {
        idx_t new_limit = prime_limit_for_idx(need);

        /* Free previous allocation if it exists. */
        if (primes != NULL) {
            free(primes);
            primes = NULL;
        }

        /* Build a new table of primes up to new_limit. */
        build_primes(new_limit, &primes, &prime_cnt);
    }

    /* primes[need‑1] is the desired prime. */
    return (seq_t)primes[(size_t)(need - 1)];
}
