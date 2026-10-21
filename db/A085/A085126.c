/*
 * A085126  Multiples of 3 which are members of A002473. Or multiples of 3
 *          with the largest prime divisor < 10.
 * https://oeis.org/A085126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3 * A002473(n) (the nth 7‑smooth number times 3)
 * Range:     u8 n <= 42, u16 n <= 438, u32 n <= 5524, u64 n <= 8759,
 *            u128 n <= 8759, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*--- static cache of 7‑smooth numbers ---------------------------------*/
    static seq_t *smooth = NULL;   /* array of 7‑smooth numbers (0‑based) */
    static idx_t   len   = 0;      /* number of values currently stored   */
    static idx_t   cap   = 0;      /* allocated capacity                  */
    static idx_t   i2 = 0, i3 = 0, i5 = 0, i7 = 0;  /* indices for each prime */

    /* initialisation on first call */
    if (len == 0) {
        cap   = 64;                     /* start with a reasonable size   */
        smooth = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!smooth) return 0;          /* memory error – return 0          */
        smooth[0] = 1;                  /* first 7‑smooth number          */
        len = 1;
    }

    /* make sure there is space for at least n values */
    if (n > cap) {
        while (n > cap) cap <<= 1;      /* double until enough capacity */
        seq_t *tmp = (seq_t *)realloc(smooth, cap * sizeof(seq_t));
        if (tmp) smooth = tmp;          /* ignore failure – return 0 later */
    }

    /* generate 7‑smooth numbers until we have n entries */
    while (len < n) {
        seq_t cand2 = smooth[i2] * ((seq_t)2);
        seq_t cand3 = smooth[i3] * ((seq_t)3);
        seq_t cand5 = smooth[i5] * ((seq_t)5);
        seq_t cand7 = smooth[i7] * ((seq_t)7);

        seq_t next = cand2;
        if (cand3 < next) next = cand3;
        if (cand5 < next) next = cand5;
        if (cand7 < next) next = cand7;

        if (cand2 == next) i2++;
        if (cand3 == next) i3++;
        if (cand5 == next) i5++;
        if (cand7 == next) i7++;

        smooth[len++] = next;
    }

    /* return the required term: 3 times the (n‑1)st 7‑smooth number */
    return smooth[n - 1] * ((seq_t)3);
}
