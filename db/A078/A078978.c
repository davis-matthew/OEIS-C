/*
 * A078978  Sequence is S(oo), where S(1)={1,2,3,4}, S(n+1)=S(n)S'(n) and
 *          S'(n) is obtained from S(n) by changing its last term using the
 *          cyclic permutation 1->2->3->4->1.
 * https://oeis.org/A078978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8997, u16 n <= 8997, u32 n <= 8997, u64 n <= 8997,
 *            u128 n <= 8997, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth term of OEIS A078978.
   The sequence is defined by:
   S(1) = {1,2,3,4},
   S(n+1) = S(n)  followed by  S'(n),
   where S'(n) is identical to S(n) except that its last element
   is replaced by the next element in the cycle 1→2→3→4→1.
   We compute a(n) by iteratively reducing the index to the base block
   [1,2,3,4], counting how many times the last element of a block is
   reached (each such occurrence cycles the value by +1 modulo 4). */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Start with the index n; terminology uses 1‑based indexing. */
    idx_t i = n;
    /* Number of times we hit the last element of a block
       (each time we must apply the cyclic permutation once).  */
    unsigned int tr = 0;          /* store modulo 4 to avoid overflow */

    /* Reduce i until it lands in the initial block 1..4. */
    while (i > 4) {
        /* Find the smallest block size L = 4·2^k that is ≥ i.
           Blocks have lengths 4,8,16,32, ... (powers of two times 4). */
        idx_t L = 4;
        while (L < i) L <<= 1;   /* L is unsigned, so safe for idx_t */

        if (i == L) {
            /* We are exactly at the last element of this block.
               Apply one cycle of the permutation and move to the
               previous block (half its size). */
            tr = (tr + 1) & 3;     /* (tr+1) mod 4 */
            i = L >> 1;
        } else if (i <= (L >> 1)) {
            /* Position lies in the first half of the block:
               value is unchanged, just continue reducing. */
            /* nothing to do */
        } else {
            /* Position lies in the second half (but not the last element):
               it equals the value of the element offset by L/2 earlier. */
            i -= (L >> 1);
        }
    }

    /* Now i is in 1..4; map it to the base values 1,2,3,4. */
    seq_t val = (seq_t)i;
    /* Apply the accumulated cyclic permutations (mod 4). */
    val = ((val - 1 + (seq_t)tr) & 3) + 1;
    return val;
}
