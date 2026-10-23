/*
 * A113623  7-smooth numbers containing only noncomposite digits (1,2,3,5,7).
 * https://oeis.org/A113623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 45, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   good_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A113623
   7‑smooth numbers (of the form 2^a 3^b 5^c 7^d) whose decimal digits belong
   to the set {1,2,3,5,7}.  a(1) = 1.  This implementation keeps a static
   list of all 7‑smooth numbers generated so far and a static list of
   accepted terms.  It generates numbers in ascending order using the
   classic four‑index method.  The function works for any call order
   and for arbitrary‑precision or fixed 64‑bit integer types. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/good_digits.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined below offset 1 */

    /* static data structures to survive across calls */
    static seq_t *smooth = NULL;
    static size_t smooth_sz = 0, smooth_cap = 0;
    static idx_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;

    static seq_t *accepted = NULL;
    static size_t acc_sz = 0, acc_cap = 0;

    /* initialise the 7‑smooth list with the first element */
    if (smooth_sz == 0) {
        smooth_cap = 4;
        smooth = (seq_t *)malloc(smooth_cap * sizeof(seq_t));
        if (!smooth) exit(1);
        smooth[0] = 1;
        smooth_sz = 1;
        /* store first accepted term if it satisfies the digit condition */
        if (good_digits(1)) {
            acc_cap = 16;
            accepted = (seq_t *)malloc(acc_cap * sizeof(seq_t));
            if (!accepted) exit(1);
            accepted[0] = 1;
            acc_sz = 1;
        }
    }

    /* generate until we have at least n accepted terms */
    while (acc_sz < n) {
        seq_t n2 = smooth[i2] * 2;
        seq_t n3 = smooth[i3] * 3;
        seq_t n5 = smooth[i5] * 5;
        seq_t n7 = smooth[i7] * 7;

        seq_t next = n2;
        if (n3 < next) next = n3;
        if (n5 < next) next = n5;
        if (n7 < next) next = n7;

        /* append next to the smooth list */
        if (smooth_sz == smooth_cap) {
            smooth_cap <<= 1;
            smooth = (seq_t *)realloc(smooth, smooth_cap * sizeof(seq_t));
            if (!smooth) exit(1);
        }
        smooth[smooth_sz++] = next;

        /* advance indices that produced this number */
        if (next == n2) ++i2;
        if (next == n3) ++i3;
        if (next == n5) ++i5;
        if (next == n7) ++i7;

        /* accept if it has allowed digits */
        if (good_digits(next)) {
            if (acc_sz == acc_cap) {
                acc_cap <<= 1;
                accepted = (seq_t *)realloc(accepted, acc_cap * sizeof(seq_t));
                if (!accepted) exit(1);
            }
            accepted[acc_sz++] = next;
        }
    }

    return accepted[n - 1];
}
