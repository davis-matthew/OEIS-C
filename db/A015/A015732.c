/*
 * A015732  Even numbers k such that d(k) | phi(k).
 * https://oeis.org/A015732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A015732:  Even integers k such that d(k) divides φ(k)
    a(n) = the n‑th positive even integer k for which   d(k) | φ(k)
    (OEIS offset 1).  The function searches forwards for such k,
    keeping a static table of found terms up to a generous bound.   */

#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"   /* size_t divisor_count_u64(uint64_t n) */
#include "../utils/calc_phi.h"           /* seq_t calc_phi(seq_t m) */

/*  Static cache of the terms.  The sequence is known to grow
    slowly; 20000 entries are enough for all n up to about 10000. */
#define MAX_TERMS 20000
static seq_t  terms[MAX_TERMS];
static idx_t term_count = 0;
static uint64_t next_candidate = 8;   /* first even integer to test */

seq_t a(idx_t n)
{
    /* Guard against 0‑based index in the routine; a(1) is the first term. */
    if (n == 0) return 0;   /* undefined, but harmless */

    /* Generate terms until we have at least n of them. */
    while (term_count < n) {
        /* Find the next even k such that d(k) | φ(k). */
        for (;;) {
            size_t d = divisor_count_u64(next_candidate);
            seq_t phi = calc_phi((seq_t)next_candidate);

            /* Convert d to seq_t for the modulus operation. */
            seq_t d_seq = d;
            if ((phi % d_seq) == 0)
                break;   /* condition satisfied */

            next_candidate += 2;    /* only even numbers are relevant */
        }

        /* Store the accepted term. */
        if (term_count < MAX_TERMS)
            terms[term_count++] = (seq_t)next_candidate;

        /* Advance to the next even candidate for the next iteration. */
        next_candidate += 2;
    }

    return terms[n - 1];
}
