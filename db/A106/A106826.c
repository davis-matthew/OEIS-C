/*
 * A106826  Trajectory of 1 under the morphism 1->{2,1}, 2->{2,3}, 3->{4,3},
 *          4->{4,1}.
 * https://oeis.org/A106826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A106826: a(n) is the n-th term of the trajectory of 1 under the morphism
   1->{2,1}, 2->{2,3}, 3->{4,3}, 4->{4,1}.  The sequence starts with a(0)=2.
   We compute a(n) by walking the binary representation of n starting from
   the root symbol 2; for each bit we descend to the first (bit 0) or second
   (bit 1) child defined by the morphism.
*/

#include <stdint.h>

/* seq_t and idx_t are supplied by the build; keep only the required
   headers. */
seq_t a(idx_t n)
{
    /* child tables indexed by the small integer value of the symbol (1..4) */
    static const idx_t first_child[5] = {0, 2, 2, 4, 4};
    static const idx_t second_child[5] = {0, 1, 3, 3, 1};

    /* a(0) is the start symbol 2 */
    if (n == 0) return (seq_t)2;

    idx_t cur = 2;          /* current symbol value */

    /* locate most‑significant bit of n */
    idx_t msb = 0;
    idx_t t = n;
    while ((t >>= 1) != 0) ++msb;

    /* descend the binary tree following the bits of n
       (most significant bit first). */
    for (idx_t i = msb;; --i) {
        if ((n >> i) & 1ULL)
            cur = second_child[cur];
        else
            cur = first_child[cur];
        if (i == 0) break;
    }

    return (seq_t)cur;
}
