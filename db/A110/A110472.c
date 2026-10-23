/*
 * A110472  Numbers n such that n in binary representation has a block of
 *          exactly a semiprime number of zeros.
 * https://oeis.org/A110472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_semiprime_uint64.h"

/* The sequence a(n) is the list of integers whose binary representation
   contains a block of consecutive zeros of exactly a semiprime length.
   The function returns the nth element of this sequence (n ≥ 1). */

/* Return true iff x has a zero block whose length is a semiprime. */
static bool has_semiprime_zero_block(uint64_t x)
{
    int run = 0;          /* current run of zeros */
    while (x != 0) {
        if ((x & 1ULL) == 0) {
            run++;        /* extend current zero run */
        } else {
            if (run > 0 && is_semiprime_uint64((uint64_t)run))
                return true;               /* found qualifying block */
            run = 0;                        /* start next run */
        }
        x >>= 1;
    }
    /* final run may end at the least‑significant bit */
    if (run > 0 && is_semiprime_uint64((uint64_t)run))
        return true;
    return false;
}

seq_t a(idx_t n)
{
    /* Cache of previously found terms */
    static seq_t *cache   = NULL;   /* dynamic array of terms */
    static idx_t  cap     = 0;      /* allocated capacity (in terms) */
    static idx_t  size    = 0;      /* number of terms stored */
    static uint64_t last = 0;        /* last integer examined */

    if (n == 0)
        return (seq_t)0;             /* defined only for n ≥ 1 */

    /* Extend cache until it contains at least n terms */
    while (size < n) {
        last++;                     /* next integer to test */

        if (has_semiprime_zero_block(last)) {
            /* Grow array if necessary */
            if (size == cap) {
                idx_t new_cap = (cap == 0) ? 4 : (cap << 1);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) {
                    /* Allocation failure – abort with zero (won't happen in tests) */
                    exit(1);
                }
                cache = tmp;
                cap   = new_cap;
            }
            cache[size++] = (seq_t)last;
        }
    }

    return cache[n - 1];
}
