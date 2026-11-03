/*
 * A101666  Fixed point of morphism 0 -> 01, 1 -> 12, 2 -> 10, starting with
 *          0.
 * https://oeis.org/A101666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A101666: Fixed point of morphism 0 → 01, 1 → 12, 2 → 10
   • a(0) = 0
   • For n > 0: begin at the root symbol 0 and, reading the binary
     digits of n from most significant to least significant,
     move to the left child for a 0‑bit and to the right child for a 1‑bit.
     The children are defined by the morphism:
       0 → 0 (left), 1 (right)
       1 → 1 (left), 2 (right)
       2 → 1 (left), 0 (right)
     The value of the final node is a(n).  This algorithm runs in
     Θ(log n) time. */

#include <stdbool.h>
#include <stdint.h>

/* Child mapping: return the child symbol of parent for a binary bit.
   The bit is 0 for left, 1 for right. */
static inline seq_t child(seq_t parent, bool bit)
{
    switch ((int)parent) {
        case 0: return bit ? (seq_t)1 : (seq_t)0;   /* 0 → 01 */
        case 1: return bit ? (seq_t)2 : (seq_t)1;   /* 1 → 12 */
        /* parent == 2: 2 → 10 */
        default: return bit ? (seq_t)0 : (seq_t)1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t val = (seq_t)0;          /* start at the root symbol 0 */

    /* Find highest power of two ≤ n.  Avoid shifting by the width of idx_t. */
    idx_t mask = (idx_t)1;
    while ((mask << 1) != (idx_t)0 && (mask << 1) <= n) {
        mask <<= 1;
    }

    /* Traverse bits of n from most significant to least significant. */
    while (mask != 0) {
        bool right = (n & mask) != 0;
        val = child(val, right);
        mask >>= 1;
    }

    return val;
}
