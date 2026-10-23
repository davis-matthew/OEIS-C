/*
 * A025052  Numbers not of form a*b + b*c + c*a for 1<=a<=b<=c (probably the
 *          list is complete).
 * https://oeis.org/A025052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, nice
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer not expressible as
   a*b + b*c + c*a for 1 <= a <= b <= c.
   Algorithm: sieve all representable numbers up to a dynamically chosen
   upper bound, then count the gaps to locate the n-th non‑representable. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* guard for non‑positive input */

    idx_t target = n;
    idx_t M = target * 40 + 1000;          /* initial upper bound estimate */

    while (1) {
        /* Allocate a bitmap of representable numbers up to M */
        unsigned char *rep = (unsigned char*)calloc((size_t)(M + 1), 1);
        if (!rep) return (seq_t)0;        /* allocation failure */

        /* Mark all numbers that can be written as a*b + b*c + c*a */
        for (idx_t a_val = 1; 3 * a_val * a_val <= M; ++a_val) {
            for (idx_t b_val = a_val; ; ++b_val) {
                idx_t base = 2 * a_val * b_val + b_val * b_val;  /* n0 = 2ab + b^2 */
                if (base > M) break;
                idx_t step = a_val + b_val;
                for (idx_t x = base; x <= M; x += step) rep[x] = 1;
            }
        }

        /* Count non‑representable numbers until we reach the desired index */
        idx_t count = 0;
        for (idx_t val = 1; val <= M; ++val) {
            if (!rep[val]) {
                ++count;
                if (count == target) {
                    free(rep);
                    return (seq_t)val;
                }
            }
        }

        free(rep);
        /* If not found, enlarge the search space and try again */
        M *= 2;
    }
}
