/*
 * A073929  a(1) = 1, a(n) = smallest number not included earlier such that
 *          the n-th partial sum (n>1) is divisible by n+1.
 * https://oeis.org/A073929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest unused integer such that * the n-th partial sum
 *            is divisible by n+1. (see OEIS A073929)
 * Range:     u8 n <= 21, u16 n <= 1968, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_seq_size
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_seq_size.h"

/* The sequence a(n). */
seq_t a(idx_t n)
{
    static seq_t *aSeq      = NULL;      /* sequence values          */
    static size_t aCap     = 0;          /* allocation size          */
    static idx_t  aLen     = 0;          /* number of terms          */

    static seq_t *usedVals  = NULL;      /* list of values already used */
    static size_t usedCap   = 0;         /* allocation size of usedVals   */
    static size_t usedCnt   = 0;         /* how many used values are stored */

    static seq_t sum = 0;                /* running sum of terms           */

    /* Initialise at start with a(1)=1. */
    if (aLen == 0) {
        ensure_seq_size(&aCap, 1, &aSeq);
        aSeq[0] = (seq_t)1;
        aLen = 1;
        sum = (seq_t)1;
        /* store the first used value */
        ensure_seq_size(&usedCap, 1, &usedVals);
        usedVals[0] = (seq_t)1;
        usedCnt = 1;
    }

    /* Compute terms on demand up to requested index n. */
    while (aLen < n) {
        idx_t idx = aLen + 1;          /* new term will be a(idx) */
        seq_t mod = (seq_t)(idx + 1);   /* divisor n + 1 */

        /* Find the remainder required for divisibility. */
        seq_t r = sum % mod;
        seq_t residue = (mod - r) % mod;   /* 0 <= residue < mod */
        /* If residue is 0 we need a positive multiple of mod. */
        if (residue == 0) residue = mod;

        /* Search for the smallest unused candidate with the right residue. */
        seq_t cand = residue;
        while (1) {
            /* Test if cand is already used by linear scan. */
            int i;
            for (i = 0; i < (int)usedCnt; ++i) {
                if (usedVals[i] == cand) break;
            }
            if (i == (int)usedCnt) break;          /* free candidate found */
            cand += mod;                           /* next number with same residue */
        }

        /* Record the chosen candidate. */
        ensure_seq_size(&aCap, aLen + 1, &aSeq);
        aSeq[aLen] = cand;
        aLen++;

        /* Store the candidate in the used list. */
        ensure_seq_size(&usedCap, usedCnt + 1, &usedVals);
        usedVals[usedCnt] = cand;
        usedCnt++;

        sum += cand;    /* update running sum */
    }

    return aSeq[n - 1];
}
