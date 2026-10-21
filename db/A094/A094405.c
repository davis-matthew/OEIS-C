/*
 * A094405  a(1) = 1; a(n) = (sum of previous terms) mod n.
 * https://oeis.org/A094405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n ≥ 2, a(n) = (sum_{i=1}^{n-1} a(i)) mod n
 * Range:     u8 n <= 30, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A094405 sequence term for a given index n.
   The function caches computed terms so that subsequent calls
  —even in a different order—are O(1) after initial computation. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state: array of computed terms, its length and allocated capacity,
       and the running sum of all computed terms. */
    static seq_t *seq = NULL;
    static size_t len = 0;   /* number of terms already stored (i.e., max index computed) */
    static size_t cap = 0;   /* allocated capacity of seq array */
    static seq_t sum = 0;    /* sum of terms a(1)..a(len) */

    if (n == 0)  /* undefined in the OEIS sense; return 0 as a safe default */
        return 0;

    /* Initialize state on first call */
    if (seq == NULL) {
        cap = 16;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        len = 0;
        sum = 0;
    }

    /* Generate terms up to n, storing them and updating the running sum. */
    while (len < n) {
        seq_t val;
        if (len == 0) {
            /* a(1) is defined to be 1 */
            val = (seq_t)1;
        } else {
            /* a(len+1) = sum of previous terms modulo (len+1) */
            val = sum % (seq_t)(len + 1);
        }

        /* Grow the array if necessary */
        if (len >= cap) {
            cap *= 2;
            seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
        }

        /* Store the new term and update the running sum */
        seq[len] = val;
        sum += val;
        ++len;
    }

    /* Return the requested term */
    return seq[n - 1];
}
