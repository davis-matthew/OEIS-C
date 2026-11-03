/*
 * A101892  a(n) = Sum_{k=0..floor(n/2)} binomial(n,2*k)*J(k), where J =
 *          A001045.
 * https://oeis.org/A101892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 2*a(n-3) + 2*a(n-4) * a(0)=0,
 *            a(1)=0, a(2)=1, a(3)=3 * Implements the linear recurrence in
 *            O(n) time with memoization
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static table grows as needed and memoizes computed values        */
    static seq_t *seq = NULL;
    static idx_t seq_size = 0;   /* number of entries stored */

    /* initialise base values on first call                         */
    if (seq_size == 0) {
        /* store terms 0..3 */
        seq_size = 4;
        seq = mem_new(seq_t, seq_size);
        if (!seq) return 0;      /* allocation failed; return 0 as safe value */
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)0;
        seq[2] = (seq_t)1;
        seq[3] = (seq_t)3;
    }

    /* extend the table if requested n is beyond current size       */
    if (n >= seq_size) {
        idx_t new_size = n + 1;
        seq_t *new_seq = mem_resize(seq_t, seq, new_size);
        if (!new_seq) return 0;   /* allocation failed; return 0 as safe value */
        seq = new_seq;
        /* compute missing values using the recurrence                */
        for (idx_t i = seq_size; i <= n; ++i) {
            /* i >= 4 here because seq_size starts at 4 and we only
             * extend when n >= seq_size.                            */
            seq_t t1 = (seq_t)4 * seq[i - 1];
            seq_t t2 = (seq_t)5 * seq[i - 2];
            seq_t t3 = (seq_t)2 * seq[i - 3];
            seq_t t4 = (seq_t)2 * seq[i - 4];
            /* arithmetic is done in seq_t.                           */
            seq_t val = t1 - t2 + t3 + t4;
            seq[i] = val;
        }
        seq_size = new_size;
    }

    return seq[n];
}
