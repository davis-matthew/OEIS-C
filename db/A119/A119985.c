/*
 * A119985  Fixed point of the morphism a -> {a, floor(3a/2), 2a}, beginning
 *          with 1.
 * https://oeis.org/A119985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: fixed point of the morphism a → {a, ⌊3a/2⌋, 2a} with a(0)=1 */

#include <stdint.h>

/* Computes the nth term of sequence A119985.
 * The term is obtained by repeatedly expanding the symbol according
 * to the morphism until the block containing the desired index is
 * reached, then following the appropriate branch.
 */
seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;

    /* Determine the size of the block (power of 3) that contains
     * index n.  Start with 1 and multiply by 3 until len > n.
     */
    idx_t len = 1;
    while (len <= n) {
        /* Prevent overflow when n is very large */
        if (len > (UINT64_MAX / 3))
            break;
        len *= 3;
    }

    seq_t val = (seq_t)1;   /* current symbol value */
    idx_t pos = n;          /* mutable index used during descent */

    while (len > 1) {
        idx_t block = len / 3;          /* size of each child block */

        if (pos < block) {
            /* first child: symbol stays the same */
        } else if (pos < 2 * block) {
            /* second child: floor(3*val/2) */
            val = (seq_t)((seq_t)3 * val) / (seq_t)2;
            pos -= block;
        } else {
            /* third child: 2*val */
            val = (seq_t)2 * val;
            pos -= 2 * block;
        }
        len = block;                   /* descend one level */
    }
    return val;
}
