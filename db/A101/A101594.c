/*
 * A101594  Numbers with exactly two distinct decimal digits, neither of which
 *          is 0.
 * https://oeis.org/A101594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less, look
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   has_two_nonzero_distinct, resize_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101594: numbers with exactly two distinct non‑zero decimal digits */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/resize_array.h"
#include "../utils/has_two_nonzero_distinct.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static state to cache already found terms */
    static seq_t *buf   = NULL;   /* array of terms found so far */
    static idx_t  sz    = 0;      /* number of stored terms */
    static idx_t  cap   = 0;      /* allocated capacity of buf        */
    static seq_t  cand  = 12;     /* next integer to test (first term) */

    /* Generate terms until we have at least n terms */
    while (sz < n) {
        if (has_two_nonzero_distinct(cand)) {
            /* Grow buffer if needed */
            if (sz == cap) {
                idx_t new_cap = cap ? cap * 2 : 16;
                seq_t *new_buf = resize_array(buf, sz, new_cap);
                if (new_buf == NULL)     /* allocation failure: abort */
                    exit(1);
                buf = new_buf;
                cap = new_cap;
            }
            buf[sz++] = cand;          /* store this term */
        }
        cand += 1;                     /* test next integer */
    }

    return buf[n - 1];                  /* return the n‑th term (1‑based) */
}
