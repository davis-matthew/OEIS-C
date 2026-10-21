/*
 * A077914  Sums of two distinct primes in exactly two ways.
 * https://oeis.org/A077914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   build_primes_upto, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This code implements A077914:
 *  a(n) = the nth smallest integer that can be written as a sum
 *  of two distinct primes in exactly two different ways.
 *
 *  The implementation builds a list of terms lazily.  The list
 *  is extended until it contains at least n elements, where
 *  n is the required index.  The extension is performed by
 *  generating all primes up to a limit, then counting how many
 *  distinct‑prime decompositions each number ≤ limit has and
 *  picking those with exactly two decompositions.
 *
 *  The algorithm is O(limit²) per extension, but the limit is
 *  increased only as needed, and the number of terms up to
 *  n = 10 000 is modest, so the computation stays well below
 *  the time limit on all supported builds.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto(idx_t limit,
                                           uint64_t **primes_out,
                                           size_t *cnt_out)           */
#include "../utils/grow_seq.h"           /* grow_seq(seq_t **p, size_t *cap,
                                            size_t new_len)              */

/* static state shared across calls -------------------------------------- */
static seq_t *terms      = NULL;   /* array of found terms */
static size_t terms_cap   = 0;     /* capacity of terms array   */
static idx_t term_count   = 0;     /* number of terms stored    */
static idx_t current_limit = 0;    /* last prime limit used     */

/* recompute the whole list of terms up to a given prime limit          */
static void recompute_terms(idx_t limit)
{
    /* free old data */
    free(terms);
    terms      = NULL;
    terms_cap  = 0;
    term_count = 0;

    /* count representations for all numbers ≤ limit */
    unsigned short *cnt = (unsigned short *) /* cast for C++ */ calloc((size_t)(limit + 1),
                                                                        sizeof(unsigned short));
    if (!cnt)
        exit(EXIT_FAILURE);                 /* out of memory */

    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        for (size_t j = i + 1; j < prime_cnt; ++j) {
            uint64_t sum = p + primes[j];
            if (sum > (uint64_t)limit)
                break;                      /* primes are sorted */
            cnt[(size_t)sum]++;
        }
    }

    /* collect the numbers that have exactly two representations */
    for (idx_t s = 2; s <= limit; ++s) {
        if (cnt[(size_t)s] == 2) {
            /* grow the output array if needed */
            grow_seq(&terms, &terms_cap, term_count + 1);
            terms[term_count++] = (seq_t)s;
        }
    }

    free(cnt);
    free(primes);
    current_limit = limit;
}

/* ensure that the list contains at least n terms */
static void ensure_terms(idx_t n)
{
    if (term_count >= n)
        return;

    /* start with a rough limit; double until we have enough terms */
    idx_t limit = current_limit ? current_limit * 2 : 400000;
    while (term_count < n) {
        recompute_terms(limit);
        if (term_count >= n)
            return;
        if (limit > UINT64_MAX / 2)
            break;                     /* avoid overflow */
        limit *= 2;
    }
}

/* public entry point --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for offset < 1 */
    ensure_terms(n);
    return terms[n - 1];
}
