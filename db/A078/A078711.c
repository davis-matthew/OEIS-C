/*
 * A078711  Sequence is S(infinity), where S(1)={1,2,3}, S(n+1)=S(n)S'(n) and
 *          S'(n) is obtained from S(n) by changing last term using the cyclic
 *          permutation 1->2->3->1.
 * https://oeis.org/A078711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9928, u16 n <= 9928, u32 n <= 9928, u64 n <= 9928,
 *            u128 n <= 9928, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078711
   S(1) = {1,2,3}
   S(n+1) = S(n) concatenated with S'(n),
   where S'(n) is S(n) with its last element cycled
   1 → 2 → 3 → 1.
   The infinite sequence is formed by repeatedly doubling
   the array until it reaches the requested index.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* lazy static cache of generated terms */
    static seq_t *seq = NULL;
    static size_t len   = 0;      /* current number of stored terms */

    if (n == 0) {          /* n should never be 0, but guard anyway */
        return (seq_t)0;
    }

    /* one‑time initialization with the base sequence S(1) */
    if (seq == NULL) {
        seq = (seq_t *)malloc(3 * sizeof(seq_t));
        if (seq == NULL) return (seq_t)0;   /* allocation failure – unlikely */
        seq[0] = 1; seq[1] = 2; seq[2] = 3;
        len = 3;
    }

    /* extend the cache until it covers index n */
    while (len < (size_t)n) {
        size_t old_len   = len;
        size_t new_len   = old_len * 2;
        seq_t *new_seq   = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (new_seq == NULL) return (seq_t)0; /* allocation failure */

        /* copy old sequence twice */
        for (size_t i = 0; i < old_len; ++i) {
            new_seq[i]          = seq[i];
            new_seq[old_len + i] = seq[i];
        }

        /* change the last element according to the 1→2→3 cycle */
        new_seq[new_len - 1] = (seq[old_len - 1] % 3) + 1;

        /* replace old array */
        free(seq);
        seq = new_seq;
        len = new_len;
    }

    return seq[n - 1];
}
