/*
 * A085127  Multiples of 4 which are members of A002473. Or multiples of 4
 *          with the largest prime divisor < 10.
 * https://oeis.org/A085127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4 * A002473(n) A002473(n) is the n-th integer composed
 *            solely of the primes 2,3,5,7 (smooth numbers)
 * Range:     u8 n <= 35, u16 n <= 399, u32 n <= 5259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the sequence lazily on a static array.  It keeps the
   multiset indices (i2,i3,i5,i7) of the next multiple of each prime,
   growing the array as needed.  Each call may request an arbitrary
   index; the array is extended only once per number, guaranteeing
   amortised O(1) per term. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static state: generated smooth numbers and indices for primes 2,3,5,7 */
    static seq_t *smooth = NULL;      /* array of A002473 terms, 0‑based */
    static size_t smooth_cap = 0;     /* allocated capacity of smooth */
    static size_t smooth_len = 0;     /* number of elements actually stored */
    static size_t i2 = 0, i3 = 0, i5 = 0, i7 = 0; /* next multiples */

    /* Guard for non‑positive indices – not used in OEIS but harmless */
    if (n <= 0) return 0;

    /* Initialise on first call */
    if (smooth_len == 0) {
        smooth_cap = 1;
        smooth = (seq_t *)malloc(smooth_cap * sizeof(seq_t));
        if (!smooth) exit(1);            /* out of memory */
        smooth[0] = 1;                    /* 1 is the first smooth number */
        smooth_len = 1;
        i2 = i3 = i5 = i7 = 0;
    }

    /* Extend the array until we have at least n terms */
    while (smooth_len < (size_t)n) {
        /* Grow storage if necessary */
        if (smooth_len >= smooth_cap) {
            size_t new_cap = smooth_cap * 2;
            if (new_cap == 0) new_cap = 4;
            smooth = (seq_t *)realloc(smooth, new_cap * sizeof(seq_t));
            if (!smooth) exit(1);
            smooth_cap = new_cap;
        }

        /* Compute next candidates */
        seq_t cand2 = smooth[i2] * 2;
        seq_t cand3 = smooth[i3] * 3;
        seq_t cand5 = smooth[i5] * 5;
        seq_t cand7 = smooth[i7] * 7;

        /* Choose the smallest candidate as the next smooth number */
        seq_t next = cand2;
        if (cand3 < next) next = cand3;
        if (cand5 < next) next = cand5;
        if (cand7 < next) next = cand7;

        /* Store the new term */
        smooth[smooth_len] = next;
        smooth_len++;

        /* Increment indices that produced this value */
        if (next == cand2) i2++;
        if (next == cand3) i3++;
        if (next == cand5) i5++;
        if (next == cand7) i7++;
    }

    /* The requested term is 4 times the (n‑th) smooth number */
    return smooth[n-1] * (seq_t)4;
}
