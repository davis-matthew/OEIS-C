/*
 * A065003  Not McNugget numbers.
 * https://oeis.org/A065003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th positive integer NOT representable as 6x+9y+20z. 
   The values are computed by a small sieve that marks representable numbers up to 100. */

#include <stdbool.h>

#define LIMIT 100            /* Enough to cover all non‑representable numbers (max is 43) */

seq_t a(idx_t n)
{
    /* Static cache of non‑representable numbers */
    static idx_t initialized = 0;
    static seq_t not_rep[100];
    static idx_t not_rep_count = 0;

    if (!initialized) {
        bool reachable[LIMIT + 1];
        for (idx_t i = 0; i <= LIMIT; ++i) reachable[i] = false;

        /* Mark all numbers reachable as 6x + 9y + 20z */
        for (idx_t z = 0; 20*z <= LIMIT; ++z) {
            for (idx_t y = 0; 9*y + 20*z <= LIMIT; ++y) {
                for (idx_t x = 0; 6*x + 9*y + 20*z <= LIMIT; ++x) {
                    reachable[6*x + 9*y + 20*z] = true;
                }
            }
        }

        /* Collect the non‑representable numbers */
        for (idx_t i = 1; i <= LIMIT; ++i) {
            if (!reachable[i]) {
                not_rep[not_rep_count++] = (seq_t)i;
                if (not_rep_count >= 100) break;   /* just in case */
            }
        }
        initialized = 1;
    }

    if (n == 0) return 0;                    /* offset guard */
    if (n - 1 < not_rep_count) return not_rep[n - 1];
    return 0;                                /* out of range – sequence ends after the largest
                                                non‑representable number (43) */
}
