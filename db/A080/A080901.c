/*
 * A080901  a(1)=2; for n>1, a(n)=a(n-1)-2 if n is already in the sequence,
 *          a(n)=a(n-1)+5 otherwise.
 * https://oeis.org/A080901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2; * For n > 1: a(n) = a(n-1)-2 if n already occurs in
 *            the sequence, * otherwise a(n) = a(n-1)+5
 * Range:     u8 n <= 217, u16 n <= 1008, u32 n <= 1008, u64 n <= 1008,
 *            u128 n <= 1008, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoises computed terms and updates a bit-array tracking
 * which integers have appeared as values.  The algorithm runs in
 * O(n) time per new term and uses O(n) memory. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static storage of computed terms                    */
    static seq_t *seq   = NULL;   /* seq[1] stores a(1)          */
    static size_t seq_cap = 0;    /* allocated size (in seq_t)   */
    static idx_t  seq_last = 0;   /* highest index computed       */

    /* Static storage of membership flags: present[i] = true
     * iff integer i already appeared as a sequence value.   */
    static uint8_t *present = NULL;   /* each byte is a flag       */
    static size_t  present_cap = 0; /* allocated size (in bytes) */

    /* --------------------------------------------------------------------- */
    /* initialise on first call                                              */
    /* --------------------------------------------------------------------- */
    if (seq_cap == 0) {
        /* allocate space for indices 0 and 1 (index 0 unused) */
        seq_cap = 2;
        seq = (seq_t*)calloc(seq_cap, sizeof(seq_t));
        if (!seq) return 0;   /* allocation failure, return 0 */
        seq[1] = (seq_t)2;    /* a(1) = 2 */
        seq_last = 1;

        /* present[2] = true for the initial value 2 */
        present_cap = 3; /* up to index 2 inclusive */
        present = (uint8_t*)calloc(present_cap, sizeof(uint8_t));
        if (!present) return 0;
        present[2] = 1;
    }

    /* --------------------------------------------------------------------- */
    /* extend capacity for sequence array if needed                          */
    /* --------------------------------------------------------------------- */
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = seq_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        /* zero the newly allocated part (optional, not needed for values) */
        memset(tmp + seq_cap, 0, (new_cap - seq_cap) * sizeof(seq_t));
        seq = tmp;
        seq_cap = new_cap;
    }

    /* --------------------------------------------------------------------- */
    /* extend capacity for membership array if needed                         */
    /* --------------------------------------------------------------------- */
    if ((size_t)n + 1 > present_cap) {
        size_t new_cap = present_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        uint8_t *tmp = (uint8_t*)realloc(present, new_cap);
        if (!tmp) return 0;
        memset(tmp + present_cap, 0, new_cap - present_cap);
        present = tmp;
        present_cap = new_cap;
    }

    /* --------------------------------------------------------------------- */
    /* compute missing terms up to index n                                    */
    /* --------------------------------------------------------------------- */
    for (idx_t i = seq_last + 1; i <= n; ++i) {
        seq_t prev = seq[i - 1];
        /* check if i is already present among previous terms */
        if (i < present_cap && present[i]) {
            seq[i] = prev - (seq_t)2;   /* a(n) = a(n-1) - 2 */
        } else {
            seq[i] = prev + (seq_t)5;   /* a(n) = a(n-1) + 5 */
        }

        /* mark the new value as present, expanding present array if needed */
        seq_t val = seq[i];
        /* only mark if the value is within the current present capacity.
           The array will be enlarged later if a higher index becomes needed. */
        if (val < (seq_t)present_cap) {
            present[(size_t)val] = 1;
        } else {
            /* enlarge present to accommodate this value */
            size_t new_cap = (size_t)val + 1;
            if (new_cap < present_cap * 2) new_cap = present_cap * 2;
            uint8_t *tmp = (uint8_t*)realloc(present, new_cap);
            if (!tmp) return 0;
            memset(tmp + present_cap, 0, new_cap - present_cap);
            present = tmp;
            present_cap = new_cap;
            present[(size_t)val] = 1;
        }
    }

    seq_last = n;
    return seq[n];
}
