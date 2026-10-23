/*
 * A107946  Start with S(0)={1}, then S(k+1) equals the concatenation of S(k)
 *          with the partial sums of S(k); the limit gives this sequence.
 * https://oeis.org/A107946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 442, u32 n <= 3684, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursively defines S(0)={1}. 
   For k≥0, S(k+1) = S(k) ⊕ (partial sums of S(k)). 
   The infinite concatenation of these S(k)’s yields sequence A107946.
   This function builds the sequence up to the requested index n on demand. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation array */
    static seq_t *buf = NULL;
    static idx_t len   = 0;      /* currently stored length (always a power of two) */
    static idx_t cap   = 0;      /* allocated capacity */

    if (n == 0)   /* sequence is 1‑based; undefined below 1, return 0 for safety */
        return 0;

    /* initialise with the first element S(0) = {1} */
    if (len == 0) {
        cap = 1;
        buf = (seq_t *)malloc(sizeof(seq_t) * cap);
        buf[0] = 1;
        len = 1;
    }

    /* extend the sequence until it contains at least n elements */
    while (len < n) {
        idx_t new_len = len * 2;

        /* grow the buffer if necessary */
        if (new_len > cap) {
            cap = new_len;
            buf = (seq_t *)realloc(buf, sizeof(seq_t) * cap);
        }

        /* compute partial sums of the current S(k) and append them */
        seq_t sum = 0;
        for (idx_t i = 0; i < len; ++i) {
            sum += buf[i];
            buf[len + i] = sum;
        }

        len = new_len;
    }

    return buf[n - 1];
}
