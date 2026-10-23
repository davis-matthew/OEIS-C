/*
 * A160522  The n-th odd composite number minus the n-th even composite
 *          number.
 * https://oeis.org/A160522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = odd composite number at position n - (2*n + 2)
 * Range:     u8 n <= 80, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The nth even composite is simply 2*n+2, since every even number ≥4 is composite. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

static seq_t *odd_comps = NULL;   /* array of odd composite numbers */
static size_t odd_comps_len = 0;  /* number of stored composites */
static size_t odd_comps_cap = 0;  /* allocated capacity */
static seq_t next_odd_candidate = 9; /* smallest odd composite to check next */

seq_t a(idx_t n)
{
    /* Make sure we have n composites stored */
    size_t target = (size_t)n;
    if (odd_comps_len < target) {
        /* Increase capacity if needed */
        if (odd_comps_cap == 0) {
            odd_comps_cap = 128;
            odd_comps = (seq_t *)malloc(odd_comps_cap * sizeof(seq_t));
        }
        while (odd_comps_len < target) {
            /* Find the next odd composite */
            seq_t cand = next_odd_candidate;
            /* Ensure cand is odd and >= 9 */
            if (cand < 9) cand = 9;
            /* Advance cand to the next odd if it's even */
            if ((cand & 1) == 0) cand++;

            /* Check for composite: not prime and >1 */
            while (isPrimeTrial(cand)) {
                cand += 2; /* skip next odd */
            }

            /* Store it */
            if (odd_comps_len == odd_comps_cap) {
                odd_comps_cap *= 2;
                odd_comps = (seq_t *)realloc(odd_comps,
                                             odd_comps_cap * sizeof(seq_t));
            }
            odd_comps[odd_comps_len++] = cand;
            next_odd_candidate = cand + 2;
        }
    }

    seq_t odd = odd_comps[n - 1];
    seq_t even = ((seq_t)n) * 2 + 2;
    return odd - even;
}
