/*
 * A123273  a(0) = 1; a(n) = the number of earlier terms, a(k), where
 *          gcd(a(k), a(floor(k/2))) = 1.
 * https://oeis.org/A123273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k < n such that gcd(a(k), a(floor(k/2))) = 1
 *            Initial value: a(0) = 1
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   alloc_seq_array, gcd, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include <stdlib.h>          /* NULL, abort */
#include "../utils/gcd.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static idx_t  len = 0;
    static seq_t  cum = 0;

    /* Initialisation */
    if (len == 0) {
        cap = 16;
        seq = alloc_seq_array(cap);
        if (!seq) abort();          /* allocation failed */
        seq[0] = (seq_t)1;
        len = 1;
        cum = (seq_t)0;
    }

    /* Extend the sequence up to index n, if necessary */
    while (len <= n) {
        /* Count whether a(len-1) is coprime with a(floor((len-1)/2)) */
        idx_t i = len - 1;
        seq_t a_i      = seq[i];
        seq_t a_floor  = seq[i / 2];
        if (gcd(a_i, a_floor) == (seq_t)1) {
            cum++;
        }

        /* Ensure capacity for the new element */
        if ((size_t)len >= cap) {
            /* double the capacity */
            grow_seq_array(&seq, &cap, cap * 2);
            if (!seq) abort();  /* allocation failed */
        }

        seq[len] = cum;
        len++;
    }

    return seq[n];
}
