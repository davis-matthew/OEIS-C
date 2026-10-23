/*
 * A191818  A sum-square avoiding sequence; fixed point of the map 0 -> 03; 1
 *          -> 43; 3 -> 1; 4 -> 01.
 * https://oeis.org/A191818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth digit of the fixed point of the morphism 0 -> 03,
 *            1 -> 43, 3 -> 1, 4 -> 01 obtained by iteratively expanding the
 *            word starting from 0 until enough terms are produced
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

#ifdef __cplusplus
/* Allocate an array of seq_t objects in C++ */

#else
/* Allocate an array of seq_t objects in C */

#endif

/* Static storage for the generated sequence. */
static seq_t *seq = NULL;      /* array of sequence values */
static size_t seq_len = 0;     /* current number of elements */

/* Grow the sequence until it contains at least the element with index n. */
static void ensure_len(idx_t n)
{
    /* If this is the first call, start with the single digit 0. */
    if (seq == NULL) {
        seq = alloc_seq(1);
        seq[0] = (seq_t)0;
        seq_len = 1;
    }

    /* Keep expanding until we have at least n+1 elements. */
    while (seq_len <= (size_t)n) {
        /* First, compute the length of the next expansion. */
        size_t new_len = 0;
        for (size_t i = 0; i < seq_len; ++i) {
            const seq_t v = seq[i];
            if (v == (seq_t)0)          new_len += 2;   /* 0 -> 03 */
            else if (v == (seq_t)1)     new_len += 2;   /* 1 -> 43 */
            else if (v == (seq_t)3)     new_len += 1;   /* 3 -> 1  */
            else if (v == (seq_t)4)     new_len += 2;   /* 4 -> 01 */
            /* no other values appear in this morphic process */
        }

        /* Allocate the new array and fill it with the expansion. */
        seq_t *new_seq = alloc_seq(new_len);
        size_t pos = 0;
        for (size_t i = 0; i < seq_len; ++i) {
            const seq_t v = seq[i];
            if (v == (seq_t)0) {
                new_seq[pos++] = (seq_t)0; new_seq[pos++] = (seq_t)3;
            } else if (v == (seq_t)1) {
                new_seq[pos++] = (seq_t)4; new_seq[pos++] = (seq_t)3;
            } else if (v == (seq_t)3) {
                new_seq[pos++] = (seq_t)1;
            } else if (v == (seq_t)4) {
                new_seq[pos++] = (seq_t)0; new_seq[pos++] = (seq_t)1;
            }
        }

        /* Replace the old array. */
        free_seq(seq);
        seq = new_seq;
        seq_len = new_len;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    ensure_len(n);
    return seq[(size_t)n];
}
