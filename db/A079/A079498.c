/*
 * A079498  Numbers whose sum of digits in base b gives 0 (mod b), for b = 3.
 * https://oeis.org/A079498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A079498
 *  a(n) = the n-th nonnegative integer m whose sum of digits in base 3 is 
 *  congruent to 0 (mod 3).  (Offset 1: a(1)=0.)
 *
 *  The implementation keeps a dynamically growing array of all values
 *  found so far so that any call order is supported efficiently.
 *  It works for all build configurations defined by seq_t and idx_t,
 *  including arbitrary‑precision seq_t.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *seq  = NULL;  /* cached sequence values   */
    static size_t count = 0;    /* number of stored terms   */
    static size_t capacity = 0; /* allocated capacity       */
    static seq_t last_val = 0;  /* last value inserted     */

    /* initialise on first call */
    if (seq == NULL) {
        capacity = 1;
        seq = (seq_t *)malloc(sizeof(seq_t) * capacity);
        seq[0] = (seq_t)0;   /* a(1) = 0 */
        count  = 1;
        last_val = (seq_t)0;
    }

    /* fast path if the requested n is already known */
    if ((size_t)n <= count) {
        return seq[n - 1];
    }

    /* extend the sequence until it contains at least n terms */
    while (count < (size_t)n) {
        /* iterate over successive candidates (last_val + 1, last_val + 2, ...) */
        seq_t cand = last_val;
        while (1) {
            cand = cand + 1;
            /* compute base‑3 digit sum modulo 3 */
            seq_t tmp = cand;
            unsigned sum_mod3 = 0;
            while (tmp > 0) {
                sum_mod3 += (unsigned)(tmp % 3);
                if (sum_mod3 >= 3) sum_mod3 -= 3;
                tmp /= 3;
            }
            if (sum_mod3 == 0) {            /* candidate satisfies the condition */
                break;
            }
        }

        /* store the found value */
        if (count == capacity) {
            capacity  = capacity * 2;
            seq = (seq_t *)realloc(seq, sizeof(seq_t) * capacity);
        }
        seq[count]  = cand;
        last_val = cand;
        ++count;
    }

    return seq[n - 1];
}
