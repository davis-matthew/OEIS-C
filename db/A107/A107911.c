/*
 * A107911  Numbers having consecutive zeros and also consecutive ones in
 *          binary representation.
 * https://oeis.org/A107911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 122, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence lists the positive integers that contain at least one
 * consecutive pair of zeros (00) **and** at least one consecutive pair of
 * ones (11) in their binary representation.
 *
 * Implementation: iterate over integers, test each with a simple bit
 * scan, and memoize all found terms so that subsequent calls are fast.
 * The function works for any seq_t type (integer or arbitrary-precision)
 * and any idx_t type (uint64_t etc.). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return true iff x has both 00 and 11 in binary. */
static bool has00_and_11(uint64_t x)
{
    if (x == 0) return false;
    bool has00 = false, has11 = false;
    int prev = -1;          /* no previous bit yet */
    while (x) {
        int bit = (int)(x & 1ULL);
        if (prev >= 0) {
            if (prev == 0 && bit == 0) has00 = true;
            if (prev == 1 && bit == 1) has11 = true;
            if (has00 && has11) return true;
        }
        prev = bit;
        x >>= 1;
    }
    return false;
}

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;    /* undefined for n < 1 */

    /* Static state for memoization. */
    static seq_t   *terms   = NULL;
    static size_t  term_cnt = 0;
    static size_t  term_cap = 0;
    static uint64_t next_val = 1;   /* next integer to test */

    /* Generate terms until we have at least n of them. */
    while (term_cnt < (size_t)n) {
        if (has00_and_11(next_val)) {
            /* Grow storage if needed. */
            if (term_cnt == term_cap) {
                size_t new_cap = term_cap ? term_cap << 1 : 32;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) {         /* allocation failed: clean up */
                    free(terms);
                    terms = NULL;
                    term_cnt = term_cap = 0;
                    return 0;
                }
                terms   = tmp;
                term_cap = new_cap;
            }
            terms[term_cnt++] = (seq_t)next_val;
        }
        ++next_val;
    }

    return terms[n - 1];
}
