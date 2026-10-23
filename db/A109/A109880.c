/*
 * A109880  Numbers k such that k and its digit reversal both are difference
 *          of successive cubes.
 * https://oeis.org/A109880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth integer k such that k = 3x²‑3x+1 and rev(k) =
 *            3y²‑3y+1 for some integers x,y ≥ 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   isqrt, rev, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/rev.h"
#include "../utils/isqrt.h"
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static storage for computed terms */
    static seq_t *seq = NULL;
    static idx_t cap  = 0;
    static idx_t size = 0;
    static idx_t cur_t = 1;  /* current t to test (t ≥ 1) */

    /* Allocate initial array if needed */
    if (seq == NULL) {
        cap  = 16;
        seq  = seq_malloc(cap);
    }

    /* Generate terms until we have at least n of them */
    while (size < n) {
        if (size == cap) {
            /* Double capacity */
            cap = cap * 2;
            seq = seq_realloc(seq, cap);
        }

        seq_t t = (seq_t)cur_t;
        seq_t k = 3 * t * t - 3 * t + 1;    /* 3t²‑3t+1 */

        seq_t revk = rev(k);
        if ((revk % 3) == 1) {
            seq_t d     = (revk - 1) / 3;
            seq_t delta = 1 + 4 * d;        /* discriminant */
            seq_t sqrtDelta = isqrt(delta);
            if (sqrtDelta * sqrtDelta == delta &&
                ((1 + sqrtDelta) % 2) == 0) {
                /* k and rev(k) are both valid differences of successive cubes */
                seq[size++] = k;
            }
        }
        cur_t++;
    }

    return seq[n - 1];
}
