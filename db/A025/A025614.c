/*
 * A025614  Numbers of form 3^i*6^j, with i, j >= 0.
 * https://oeis.org/A025614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest number of the form 3^i * 6^j, i, j ≥ 0 (n ≥ 1)
 * Range:     u8 n <= 12, u16 n <= 39, u32 n <= 142, u64 n <= 533,
 *            u128 n <= 2064, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm generates the sequence by merging two increasing subsequences:
 *   • candidates from multiplying previously generated terms by 3
 *   • candidates from multiplying previously generated terms by 6
 * This guarantees that each term appears exactly once and the terms are output in ascending order.
 */

#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Internal storage for the sequence                                     */
/* --------------------------------------------------------------------- */
static seq_t *seq          = NULL;   /* array of generated terms          */
static size_t seq_len      = 0;      /* number of terms stored            */
static size_t seq_cap      = 0;      /* allocated capacity (in elements)  */
static idx_t    next_by_3  = 0;      /* next index for multiplying by 3   */
static idx_t    next_by_6  = 0;      /* next index for multiplying by 6   */
static int      initialized = 0;     /* flag to trigger one‑time init      */

/* --------------------------------------------------------------------- */
/*  Ensure that the array has sufficient space for at least 'needed'    */
/* --------------------------------------------------------------------- */
static void ensure_capacity(size_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        while (new_cap < needed) new_cap *= 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/*  Main accessor routine                                                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* undefined for n = 0 */

    /* first time: create the first term and initialise indices */
    if (!initialized) {
        ensure_capacity(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
        next_by_3 = 0;
        next_by_6 = 0;
        initialized = 1;
    }

    /* Generate successive terms until we have at least 'n' of them */
    while (seq_len < (size_t)n) {
        seq_t cand3 = seq[next_by_3] * (seq_t)3;
        seq_t cand6 = seq[next_by_6] * (seq_t)6;
        seq_t next;

        if (cand3 < cand6) {
            next = cand3;
            next_by_3++;
        } else if (cand6 < cand3) {
            next = cand6;
            next_by_6++;
        } else {                /* equal: produce once, advance both pointers */
            next = cand3;
            next_by_3++;
            next_by_6++;
        }

        ensure_capacity(seq_len + 1);
        seq[seq_len++] = next;
    }

    return seq[(size_t)n - 1];
}
