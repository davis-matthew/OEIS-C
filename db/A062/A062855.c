/*
 * A062855  First differences of A062851.
 * https://oeis.org/A062855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A062855:
   a(n) = 2*n-1 - (number of new product entries introduced by factor n).
   The sequence counts the first differences in the number of integers that
   are not representable as a product i*j with 1 ≤ i,j ≤ n.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static state for memoisation */
    static idx_t max_n = 0;          /* largest n computed so far          */
    static seq_t *a_vals = NULL;    /* a_vals[1..max_n]                    */
    static uint64_t *bits = NULL;   /* bitset of products up to max_n^2   */
    static idx_t bits_len = 0;      /* number of uint64_t words in bits    */

    if (n == 0) return 0;            /* offset is 1, but guard against 0 */

    /* extend precomputation if necessary */
    if (n > max_n) {
        /* ---- resize bitset to cover n^2 indices ---- */
        idx_t target_bits = n * n;                    /* maximum index needed   */
        idx_t target_len = (target_bits >> 6) + 1;    /* words needed           */
        if (target_len > bits_len) {
            uint64_t *new_bits = (uint64_t *)realloc(bits,
                                                      target_len * sizeof(uint64_t));
            /* if realloc expanded, initialise new words to 0   */
            for (idx_t i = bits_len; i < target_len; ++i) new_bits[i] = 0;
            bits = new_bits;
            bits_len = target_len;
        }

        /* ---- resize array of a(n) values ---- */
        if (n + 1 > max_n + 1) { /* realloc must be large enough to hold indices up to n */
            seq_t *new_a_vals = (seq_t *)realloc(a_vals,
                                                 (n + 1) * sizeof(seq_t));
            /* initialise new part (unused) to zero – not strictly required */
            for (idx_t i = max_n + 1; i <= n; ++i) new_a_vals[i] = 0;
            a_vals = new_a_vals;
        }

        /* ---- compute missing a(k) for k = max_n+1 .. n ---- */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            /* size of new range introduced by n = k   */
            seq_t new_range = (seq_t)(2 * (seq_t)k - 1);

            seq_t newly_marked = 0;
            for (idx_t j = 1; j <= k; ++j) {
                idx_t m = k * j;                 /* product i*j   */
                idx_t word = m >> 6;             /* word index    */
                uint64_t mask = 1ULL << (m & 63);/* bit mask      */
                if (!(bits[word] & mask)) {      /* not yet marked? */
                    bits[word] |= mask;          /* mark it        */
                    ++newly_marked;
                }
            }
            a_vals[k] = new_range - newly_marked;
        }
        max_n = n;
    }
    return a_vals[n];
}
