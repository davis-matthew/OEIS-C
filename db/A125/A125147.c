/*
 * A125147  a(0)=1; for n >= 1, a(n) is the smallest positive integer not
 *          occurring earlier in the sequence such that Sum_{k=0..n} a(k) is a
 *          multiple of n.
 * https://oeis.org/A125147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 540, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _grow_used
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A125147.
   a(0) = 1.
   For n ≥ 1, a(n) is the smallest positive integer not occurring earlier
   in the sequence such that Sum_{k=0..n} a(k) is a multiple of n. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/_grow_used.h"

static void _grow_seq(idx_t upto, seq_t **seq_ptr, idx_t *cap_ptr) {
    while (*cap_ptr <= upto) {
        idx_t new_cap = (*cap_ptr) ? (*cap_ptr) * 2 : 1;
        while (new_cap <= upto) new_cap *= 2;
        seq_t *new_seq = (seq_t *)realloc(*seq_ptr, new_cap * sizeof(seq_t));
        if (!new_seq) exit(1);
        *seq_ptr = new_seq;
        *cap_ptr = new_cap;
    }
}

seq_t a(idx_t n) {
    static seq_t *seq = NULL;          /* array of computed values   */
    static idx_t seq_cap = 0;          /* capacity of seq array      */
    static idx_t seq_len = 0;          /* next free position         */
    static seq_t sum = 0;              /* running sum of seq[0..i-1] */

    static bool *used = NULL;          /* bool array for used numbers*/
    static idx_t used_cap = 0;         /* capacity of used array     */

    if (n < seq_len)
        return seq[n];

    /* Ensure we have space to compute up to n */
    _grow_seq(n, &seq, &seq_cap);

    /* Compute missing terms up to n */
    for (idx_t i = seq_len; i <= n; ++i) {
        if (i == 0) {                      /* initialise a(0) */
            seq[0] = (seq_t)1;
            _grow_used(1, &used, &used_cap);
            used[1] = true;
            sum = seq[0];
        } else {
            /* Find smallest unused m with (sum + m) % i == 0 */
            seq_t rem = sum % (seq_t)i;
            seq_t cand = (rem == 0) ? (seq_t)i : (seq_t)i - rem;
            if (cand == 0) cand = (seq_t)i;   /* protect against zero */

            while (1) {
                idx_t idx = (idx_t)cand;
                _grow_used(idx, &used, &used_cap);
                if (!used[idx])
                    break;
                cand += (seq_t)i;
            }
            seq[i] = cand;
            used[(idx_t)cand] = true;
            sum += cand;
        }
    }

    seq_len = n + 1;
    return seq[n];
}
