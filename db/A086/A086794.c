/*
 * A086794  Numbers k such that the k-th cyclotomic polynomial has exactly 9
 *          nonzero terms.
 * https://oeis.org/A086794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A086794: numbers k such that the k‑th cyclotomic polynomial φₖ(x)
   has exactly 9 nonzero terms.  These numbers are precisely the integers
   of the form 2^a · 3^b · 7^c with b ≥ 1 and c ≥ 1, sorted in ascending
   order.  The function generates the sequence lazily using the classic
   “humble numbers” algorithm for the three primes 2, 3, 7. */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *seq = NULL;      /* array of generated terms          */
    static size_t seq_len = 0;     /* current number of stored terms    */
    static size_t seq_cap = 0;     /* capacity of the array             */
    static size_t idx2 = 0;        /* next index for multiplication by 2 */
    static size_t idx3 = 0;        /* next index for multiplication by 3 */
    static size_t idx7 = 0;        /* next index for multiplication by 7 */

    if (n == (idx_t)0) return 0;  /* defined only for n ≥ 1, but harmless */

    /* initialise on first use */
    if (seq == NULL) {
        seq_cap = 32;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)21;  /* 3 · 7, the smallest admissible k */
        seq_len = 1;
    }

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        if (seq_len == seq_cap) {            /* grow the array when needed */
            seq_cap *= 2;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }

        seq_t c2 = seq[idx2] * (seq_t)2;
        seq_t c3 = seq[idx3] * (seq_t)3;
        seq_t c7 = seq[idx7] * (seq_t)7;

        seq_t next = c2;
        if (c3 < next) next = c3;
        if (c7 < next) next = c7;

        seq[seq_len++] = next;

        if (c2 == next) ++idx2;
        if (c3 == next) ++idx3;
        if (c7 == next) ++idx7;
    }

    return seq[(size_t)n - 1];
}
