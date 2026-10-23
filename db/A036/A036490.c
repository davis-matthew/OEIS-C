/*
 * A036490  Numbers whose prime factors are in {5, 7, 11}.
 * https://oeis.org/A036490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 58, u32 n <= 349, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers whose prime factors are in {5, 7, 11}
 * (Hamming‑type numbers) computed by a three‑pointer merge. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */

    static seq_t *seq  = 0;         /* cached sequence (including sentinel) */
    static size_t size = 0;         /* number of stored elements (incl. sentinel) */
    static size_t cap   = 0;        /* allocated capacity of seq[] */

    /* indices for the next candidate from each prime */
    static size_t ptr5  = 0;
    static size_t ptr7  = 0;
    static size_t ptr11 = 0;

    /* initialise cache once */
    if (!seq) {
        cap  = 16;                  /* arbitrary initial capacity */
        seq  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return 0;         /* allocation failure */
        seq[0] = 1;                 /* sentinel value */
        size  = 1;
    }

    /* extend cache until we have computed a(n) */
    while (size <= (size_t)n) {
        seq_t cand5  = seq[ptr5]  * 5;
        seq_t cand7  = seq[ptr7]  * 7;
        seq_t cand11 = seq[ptr11] * 11;
        seq_t next   = cand5;
        if (cand7 < next)   next = cand7;
        if (cand11 < next)  next = cand11;

        if (size >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) return 0;    /* allocation failure */
            seq = tmp;
            cap = new_cap;
        }

        seq[size++] = next;
        if (cand5  == next) ++ptr5;
        if (cand7  == next) ++ptr7;
        if (cand11 == next) ++ptr11;
    }

    return seq[n];                  /* skip the sentinel at index 0 */
}
