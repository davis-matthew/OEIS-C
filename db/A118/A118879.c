/*
 * A118879  Let T(S,Q) be the sequence obtaining by starting with S and
 *          repeatedly reversing the digits and adding Q to get the next term.
 *          This is T(1016,5), the first S for which T(S,5) reaches a cycle of
 *          length 36.
 * https://oeis.org/A118879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1016 a(n+1) = reverse_digits(a(n)) + 5 where
 *            reverse_digits(x) is the decimal reversal of x. The sequence
 *            enters a cycle of length 36 starting at index 0
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   reverse_digits_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/reverse_digits_seq_t.h"

/* Return the n‑th term of A118879.  The function lazily builds the
   initial segment until the first repetition, records the cycle
   length, and then answers any index using modular arithmetic. */
seq_t a(idx_t n)
{
    /* Static state: cache of terms, its capacity, and cycle info. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t len = 0;
    static idx_t cycle_start = 0;   /* first index of the cycle   */
    static idx_t cycle_len   = 0;   /* length of the cycle        */

    /* First call → initialise cache and find cycle. */
    if (len == 0) {
        /* Allocate initial capacity. */
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 1016U;          /* a(0) */
        len = 1;

        /* Generate terms until a repeated value is seen. */
        while (1) {
            seq_t cur = cache[len - 1];
            seq_t nxt = reverse_digits_seq_t(cur) + 5U;

            /* Linear search to detect repetition. */
            idx_t j;
            for (j = 0; j < len; ++j) {
                if (cache[j] == nxt) break;
            }
            if (j < len) {
                /* Cycle found: terms j .. len-1 form the cycle. */
                cycle_start = j;
                cycle_len   = len - j;
                break;
            }

            /* Store the new term, expanding cache if necessary. */
            if (len == (idx_t)cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = nxt;
        }
    }

    /* Return precomputed term if it exists. */
    if (n < len) {
        return cache[n];
    }

    /* For indices beyond the precomputed segment, use cycle
       repetition.  cycle_start may be >0; the first cycle term is at
       cycle_start. */
    idx_t idx = cycle_start + (n - cycle_start) % cycle_len;
    return cache[idx];
}
