/*
 * A161220  The n-th member of a twin prime pair minus 2*n.
 * https://oeis.org/A161220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001097(n) – A005843(n) where A001097(n) is the n‑th odd
 *            prime that belongs to a twin prime pair and A005843(n) is 2*n.
 *            Thus a(n) = p_n – 2*n with p_n as described
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, free */
#include <stdint.h> /* uint64_t, size_t */
#include "../utils/build_primes_upto.h"

/* typed integer declarations (supplied by the build) */
extern seq_t      a(idx_t n);   /* forward declaration for the compiler */

static seq_t*       twin_primes = NULL;   /* array of all primes that are
                                            part of a twin pair (ascending) */
static size_t       twin_count  = 0;       /* number of stored twin primes   */
static idx_t        prime_limit = 0;       /* current sieve upper bound      */

/* Ensure the twin_primes array contains at least 'n' elements. */
static void
ensure_twin_primes(idx_t n)
{
    /* Nothing to do if we already have enough. */
    if (twin_count >= (size_t)n)
        return;

    /* Initialize the sieve limit if this is the first call. */
    if (prime_limit == 0)
        prime_limit = 1000;

    /* Extend the limit until enough twin primes are found. */
    while (twin_count < (size_t)n) {
        /* Double the limit each iteration. */
        prime_limit <<= 1;

        /* Generate all primes up to the current limit. */
        uint64_t *prime_list = NULL;
        size_t    prime_cnt  = 0;
        build_primes_upto(prime_limit, &prime_list, &prime_cnt);

        /* Allocate a temporary array large enough to hold all
           twin‑related primes (worst case: each prime is counted). */
        seq_t *tmp = (seq_t *)malloc(2 * prime_cnt * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failure – abort with diagnostic (not expected). */
            abort();
        }
        size_t tcnt = 0;

        /* Scan the primes and collect every prime that is part of a
           twin pair.  Two consecutive primes p[i] and p[i+1] with
           difference 2 form a twin‑prime pair.  A prime appears in
           the output once iff it is adjacent to such a pair. */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            bool hasPrev = (i > 0) && ((prime_list[i - 1] + 2ULL) == p);
            bool hasNext = (i + 1 < prime_cnt) && ((p + 2ULL) == prime_list[i + 1]);

            if (hasPrev || hasNext) {
                tmp[tcnt++] = (seq_t)p;
            }
        }

        /* Replace the old array.  We free the temporary array if
           we have already allocated a previous one. */
        if (twin_primes != NULL)
            free(twin_primes);
        twin_primes = tmp;
        twin_count  = tcnt;

        free(prime_list);
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* safety for non‑positive inputs */

    /* Lazily build or extend the twin‑prime list up to the required index. */
    ensure_twin_primes(n);

    /* The n-th term of the sequence is the n-th twin prime minus 2*n. */
    seq_t twin_val = twin_primes[(size_t)n - 1];
    seq_t nn = (seq_t)n;
    return twin_val - (nn << 1);   /* same as twin_val - 2*n */
}
