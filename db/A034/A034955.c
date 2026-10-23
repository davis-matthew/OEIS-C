/*
 * A034955  Even triangular numbers with prime indices.
 * https://oeis.org/A034955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000217(A002145(n)) (triangular number of the n‑th prime
 *            p ≡ 3 (mod 4) )
 * Range:     u8 n <= 4, u16 n <= 37, u32 n <= 4091, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *p3 = NULL;          /* primes ≡ 3 (mod 4), 1‑based index */
static size_t   p3cnt = 0;           /* number of stored primes          */
static idx_t    sieve_limit = 0;     /* last limit used for sieving      */

/* Ensure that the array `p3` contains at least `need` primes of the
   form 4*k+3.  The array grows only when necessary. */
static void
ensure_p3(idx_t need)
{
    if (p3cnt >= (size_t)need)
        return;                       /* already have enough */

    /* start with a small limit and double until enough primes are
       found.  The sieve_limit is kept so that we never reduce the
       limit, guaranteeing monotone behaviour on subsequent calls. */
    idx_t limit = sieve_limit ? sieve_limit : 1000;

    while (1) {
        uint64_t *tmp_primes = NULL;
        size_t   tmp_cnt = 0;

        build_primes_upto(limit, &tmp_primes, &tmp_cnt);

        /* count how many satisfy p ≡ 3 (mod 4) */
        size_t cnt3 = 0;
        for (size_t i = 0; i < tmp_cnt; ++i)
            if (tmp_primes[i] % 4 == 3)
                ++cnt3;

        if (cnt3 >= (size_t)need) {
            /* allocate a new array for the qualifying primes */
            uint64_t *newp3 = (uint64_t *)calloc(cnt3, sizeof(uint64_t));
            if (!newp3) {
                free(tmp_primes);
                abort();               /* out of memory */
            }

            /* copy the primes ≡ 3 (mod 4) into the new array */
            size_t copy_idx = 0;
            for (size_t i = 0; i < tmp_cnt; ++i) {
                if (tmp_primes[i] % 4 == 3)
                    newp3[copy_idx++] = tmp_primes[i];
            }

            free(tmp_primes);          /* ditch temporary list */
            free(p3);                   /* discard old array */
            p3 = newp3;
            p3cnt = cnt3;
            sieve_limit = limit;        /* remember highest limit used */
            break;                      /* we now have enough primes */
        }

        /* insufficient primes: discard temp array and enlarge the limit */
        free(tmp_primes);
        limit *= 2;
    }
}

/* Return the n-th term of OEIS sequence A034955 (indexing starts at 1). */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1, return 0 */

    ensure_p3(n);

    uint64_t p = p3[n - 1];            /* n‑th qualifying prime (1‑based) */
    seq_t result = (seq_t)p * ((seq_t)p + 1) / 2;
    return result;
}
