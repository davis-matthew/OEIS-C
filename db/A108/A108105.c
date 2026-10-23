/*
 * A108105  2^floor(n/5).
 * https://oeis.org/A108105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^floor(n/5)
 * Range:     u8 n <= 39, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *pow2 = NULL;
    static idx_t size = 0;
    static idx_t cap  = 0;

    idx_t shift = n / 5;          /* floor(n/5) */

    /* initial allocation */
    if (pow2 == NULL) {
        cap  = 64;
        pow2 = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!pow2) return 0;      /* allocation failure – unlikely in tests */
        pow2[0] = (seq_t)1;
        size = 1;
    }

    /* grow capacity if needed */
    if (shift >= cap) {
        idx_t new_cap = cap * 2;
        if (new_cap <= shift) new_cap = shift + 1;
        seq_t *tmp = (seq_t *)realloc(pow2, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure */
        pow2 = tmp;
        cap  = new_cap;
    }

    /* compute missing powers up to 'shift' */
    if (shift >= size) {
        for (idx_t i = size; i <= shift; ++i) {
            pow2[i] = pow2[i - 1] * ((seq_t)2);
        }
        size = shift + 1;
    }

    return pow2[shift];
}
