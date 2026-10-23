/*
 * A109019  Numbers whose digit reversal is different and has the same number
 *          of prime factors (with multiplicity).
 * https://oeis.org/A109019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number whose digit reversal is different and has the same
 *            number of prime factors (with multiplicity)
 * Range:     u8 n <= 18, u16 n <= 2846, u32 n <= 9887, u64 n <= 9887,
 *            u128 n <= 9887, big any n
 * Helpers:   bigomega_uint64, push_seq, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev.h"
#include "../utils/bigomega_uint64.h"
#include "../utils/push_seq.h"

seq_t a(idx_t n)
{
    /* Handle non‑positive indices simply */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of computed terms */
    static seq_t *terms   = NULL;   /* array of terms */
    static idx_t  terms_cap = 0;    /* current capacity of array */
    static idx_t  terms_size = 0;   /* number of terms stored */

    /* Static seed for next candidate to test */
    static seq_t next_candidate = 10;  /* start from the smallest two‑digit number */

    /* Generate terms on demand until we have the nth term */
    while (terms_size < n)
    {
        seq_t  cand   = next_candidate++;
        seq_t  rev_cand = rev(cand);

        if (cand == rev_cand)   /* skip palindromes */
            continue;

        /* Count prime factors (with multiplicity) for both numbers.
           bigomega_uint64 works on uint64_t; we assume the numbers fit in it. */
        seq_t omega_cand   = (seq_t)bigomega_uint64((uint64_t)cand);
        seq_t omega_rev    = (seq_t)bigomega_uint64((uint64_t)rev_cand);

        if (omega_cand == omega_rev)
            push_seq(&terms, &terms_cap, &terms_size, cand);
    }

    /* Return the requested term (0‑based indexing in the array). */
    return terms[n - 1];
}
