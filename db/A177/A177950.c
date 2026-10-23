/*
 * A177950  Numbers k that divide the digit reversal of k^2.
 * https://oeis.org/A177950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 81, u32 n <= 171, u64 n <= 171,
 *            u128 n <= 171, big any n
 * Helpers:   rev_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A177950: numbers k such that k divides the digit reversal of k^2
   Algorithm: brute‑force search.  For each candidate k, compute the decimal
   reversal of k² digit by digit and test if it is divisible by k.
   The sequence is stored in a static dynamic array so that calls with
   arbitrary indices work in any order.  The routine works for all
   supported seq_t types; for small types it only stores values that fit. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/rev_mod.h"

seq_t a(idx_t n)
{
    /* static data for memoisation */
    static seq_t *seq = NULL;        /* array of found terms   */
    static size_t seq_len = 0;       /* number stored so far   */
    static size_t seq_cap = 0;       /* allocated capacity     */
    static idx_t next_candidate = 1; /* next integer to test   */

    while (seq_len < n) {            /* generate until we have nth term */
        uint64_t cand64 = (uint64_t)next_candidate;
        uint64_t sq = cand64 * cand64;

        /* test property */
        if (rev_mod(cand64, sq) == 0) {
            /* only store if candidate fits into seq_t */
            seq_t val = (seq_t)cand64;
            if ((uint64_t)val == cand64) {
                if (seq_len == seq_cap) {              /* grow array */
                    size_t new_cap = seq_cap ? seq_cap * 2 : 4;
                    seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
                    seq_cap = new_cap;
                }
                seq[seq_len++] = val;
            }
        }
        ++next_candidate;          /* advance to next integer */
    }

    return seq[n - 1];             /* return requested term (1‑based) */
}
