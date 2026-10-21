/*
 * A074264  Values of Kolakoski sequence A000002 at positions n = 0 mod 3.
 * https://oeis.org/A074264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = value of the Kolakoski sequence A000002 at index 3*n
 *            (with 1‑based indexing). The Kolakoski sequence alternates runs
 *            of 1's and 2's. This implementation generates the sequence
 *            lazily up to the requested length and returns the desired term
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 3*n is the target index (1‑based).  Since a(1) corresponds to
       the 3rd term of A000002, we need the term at position 3*n.  */
    idx_t target = 3 * n;                 /* number of terms to generate */
    idx_t return_index = target - 1;      /* 0‑based index to return */

    /* Static buffer and its capacity.  The buffer is grown only
       when more terms are required.  */
    static seq_t *buf = NULL;
    static size_t cap = 0;

    if (cap < target) {
        size_t new_cap = target > 64 ? target : 64;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* out of memory: give 0 */
        buf = tmp;
        cap = new_cap;
    }

    /* Generate the Kolakoski sequence up to 'target' terms.
       We regenerate from scratch on each call: the cost is
       O(target), which is fine for the required limits. */
    buf[0] = (seq_t)1;                    /* first element is always 1 */
    size_t i = 1;                          /* next position to fill */
    size_t j = 0;                          /* run‑length index to read */
    seq_t val = (seq_t)1;                  /* current run value (1 or 2) */

    while (i < target) {
        seq_t run = buf[j++];              /* read run length */
        for (size_t k = 0; k < run && i < target; ++k) {
            buf[i++] = val;
        }
        val = (seq_t)(3 - val);            /* toggle between 1 and 2 */
    }

    return buf[return_index];
}
