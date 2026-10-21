/*
 * A067150  Number of integers i=1,2,...,n such that (n,i) has Property F3*,
 *          i.e., i and n are consecutive terms of a sequence b(k) satisfying
 *          b(1)=1, b(n) = (b(n-1) OR 2*b(n-1)) + b(n-2), where the OR is
 *          taken bitwise.
 * https://oeis.org/A067150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct predecessors of n in any sequence b(k)
 *            with b(1)=1 and b(k) = (b(k−1) OR 2*b(k−1)) + b(k−2)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *a_vals = NULL;           /* a(n) values              */
static uint64_t **bitsets = NULL;      /* per‑n predecessor bitsets*/
static idx_t  max_generated = 0;        /* computed up to this n    */
static size_t words_per = 0;           /* number of 64‑bit words per bitset */

/* generate all admissible pairs (prev,cur) with cur<=limit
   and fill a_vals/bitsets appropriately                    */
static void compute_upto(idx_t limit)
{
    /* free previous data if necessary */
    if (a_vals) free(a_vals);
    if (bitsets) {
        for (idx_t i = 0; i <= max_generated; ++i)
            free(bitsets[i]);
        free(bitsets);
    }

    max_generated = limit;
    a_vals = (seq_t *)calloc((size_t)(max_generated + 1), sizeof(seq_t));
    bitsets = (uint64_t **)calloc((size_t)(max_generated + 1), sizeof(uint64_t *));
    words_per = ((size_t)max_generated + 63) / 64 + 1;   /* safety padding */

    for (idx_t a = 1; a <= limit; ++a) {                /* second term = a      */
        idx_t prev = 1;
        idx_t cur  = a;                                 /* first step (skip this pair) */
        while (1) {
            /* compute next term of the sequence   */
            idx_t nxt = (cur | (cur << 1)) + prev;
            if (nxt > limit) break;
            /* record pair (cur, nxt) -- nxt is the new current term   */
            if (bitsets[nxt] == NULL) {
                bitsets[nxt] = (uint64_t *)calloc(words_per, sizeof(uint64_t));
            }
            idx_t bit = cur - 1;
            size_t w = bit >> 6;
            uint64_t mask = 1ULL << (bit & 63);
            if (!(bitsets[nxt][w] & mask)) {
                bitsets[nxt][w] |= mask;
                ++a_vals[nxt];
            }
            /* advance to next step              */
            prev = cur;
            cur  = nxt;
        }
    }
}

/* external function required by the framework */
seq_t a(idx_t n)
{
    if (a_vals == NULL || n > max_generated)
        compute_upto(n);
    return a_vals[n];
}
