/*
 * A070825  One half of product of first n+1 Lucas numbers A000032.
 * https://oeis.org/A070825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1;  L(0)=2, L(1)=1;  L(n)=L(n‑1)+L(n‑2);  a(n)=a(n‑1)*L(n) */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* static cache and state */
    static seq_t *a_seq   = NULL;   /* cache of a(k) */
    static idx_t  max_n   = 0;      /* highest n for which a_seq[0..max_n] is valid */
    static size_t cap     = 0;      /* current allocation size (elements) */

    /* Lucas number state for the last computed index */
    static seq_t L_prev = (seq_t)2; /* L(0)   */
    static seq_t L_curr = (seq_t)1; /* L(1)   */
    static int    initialized = 0;

    /* first time: initialise cache */
    if (!initialized) {
        cap = 1;
        a_seq = seq_alloc_new_array(cap);
        a_seq[0] = (seq_t)1;   /* a(0) = 1 */
        max_n = 0;
        initialized = 1;
    }

    /* grow cache if needed (in C, size_t is guaranteed to hold the index count) */
    if ((size_t)(n + 1) > cap) {
        seq_t *new_seq = seq_alloc_new_array((size_t)(n + 1));
        for (idx_t i = 0; i <= max_n; ++i)
            new_seq[i] = a_seq[i];
        seq_free_array(a_seq);
        a_seq = new_seq;
        cap   = (size_t)(n + 1);
    }

    /* compute missing terms up to n */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i > 1) {                     /* update Lucas numbers for i>=2 */
                seq_t nextL = L_prev + L_curr;
                L_prev = L_curr;
                L_curr = nextL;
            }
            a_seq[i] = a_seq[i - 1] * L_curr; /* a(i) = a(i-1) * L(i) */
        }
        max_n = n;
    }

    return a_seq[n];
}
