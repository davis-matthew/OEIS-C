/*
 * A102830  "True already", base 4, start 1: a(n) is the least integer such
 *          that the sequence up to a(n-1) written in base 4 contains
 *          floor(a(n)/4) copies of the digit a(n) % 4, with a(0) = 1.
 * https://oeis.org/A102830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A102830  ("True already")
   a(0) = 1
   For n > 0 a(n) is the least integer such that the base‑4 digits of
   a(0)..a(n-1) contain exactly floor(a(n)/4) copies of the digit
   a(n) % 4. This implementation builds the sequence incrementally
   and caches all computed terms.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state: sequence, its length, capacity, last value, digit counts   */
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;
    static idx_t seq_cap = 0;
    static seq_t last_val = 0;
    static unsigned int digit_cnt[4] = {0};

    /* initialize the array on first call */
    if (seq_len == 0) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        /* first term a(0) = 1 */
        seq[0] = 1;
        seq_len = 1;
        last_val = 1;
        /* count digits of 1 in base 4 */
        digit_cnt[1] = 1;   /* only digit 1 */
    }

    /* extend the computed prefix until n is available */
    while (seq_len <= n) {
        seq_t cand = last_val + 1;      /* candidate starts at last term + 1   */
        unsigned int required;
        unsigned int digit = (unsigned int)(cand & 3);   /* cand % 4 */
        /* search for the least candidate that satisfies the condition   */
        while (1) {
            required = (unsigned int)(cand >> 2);          /* floor(cand/4) */
            if (digit_cnt[digit] == required)
                break;
            ++cand;
            digit = (unsigned int)(cand & 3);
        }

        /* store the new term */
        if (seq_len >= seq_cap) {
            seq_cap *= 2;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len] = cand;
        last_val = cand;
        ++seq_len;

        /* update digit counts with the base‑4 representation of candidate */
        seq_t tmp = cand;
        /* Note: tmp > 0 for all terms except the initial term 1.
           Base‑4 representation: while (tmp > 0) { d = tmp & 3; ...; tmp >>= 2; }   */
        while (tmp > 0) {
            unsigned int d = (unsigned int)(tmp & 3);
            digit_cnt[d]++;          /* cnt could fit in 32 bits for current n */
            tmp >>= 2;
        }
    }

    return seq[n];
}
