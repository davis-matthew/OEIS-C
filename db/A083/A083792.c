/*
 * A083792  Lexicographically first increasing sequence such that no two
 *          successive terms have the same prime signature.
 * https://oeis.org/A083792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th term of the lexicographically first strictly
 *            increasing sequence of positive integers such that two
 *            consecutive terms never share the same prime signature. No
 *            closed formula is known; this implementation constructs the
 *            sequence by incremental search
 * Range:     u8 n <= 219, u16 n <= 9744, u32 n <= 9744, u64 n <= 9744,
 *            u128 n <= 9744, big any n
 * Helpers:   same_signature, signature_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/same_signature.h"
#include "../utils/signature_uint64.h"

/* ----- Helper: prime signature of a number (unsigned 64‑bit) ----- */

/* ----- Main function ----------------------------------------------------- */

seq_t a(idx_t n)
{
    /* a(1) is defined to be 1. */
    if (n == 1) return (seq_t)1;

    seq_t prev = (seq_t)1;
    uint8_t prev_sig[64] = {0};
    size_t prev_cnt = 0;          /* signature of 1 is empty */

    for (idx_t pos = 2; pos <= n; ++pos) {
        seq_t cand = prev + 1;    /* next integer after previous term */

        while (true) {
            /* Obtain prime signature of candidate. The value is
               known to fit in uint64_t; conversion is safe. */
            uint64_t cval = (uint64_t)cand;
            uint8_t cand_sig[64];
            size_t cand_cnt;
            signature_uint64(cval, cand_sig, &cand_cnt);

            /* Candidate acceptable if its signature differs from
               that of the previous term. */
            if (!same_signature(prev_sig, prev_cnt,
                                cand_sig, cand_cnt))
                break;          /* suitable next term found */

            cand = cand + 1;    /* try the next integer */
        }

        /* Update previous term and its signature. */
        prev = cand;
        uint64_t pval = (uint64_t)prev;
        signature_uint64(pval, prev_sig, &prev_cnt);
    }

    return prev;
}
