/*
 * A065628  Permutation of N induced by rotating the node 2 left in the
 *          infinite planar binary tree. The second row of A065626. Inverse of
 *          A065627.
 * https://oeis.org/A065628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permutation induced by left rotating node 2 in the
 *            infinite planar binary tree
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A065628 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)n;          /* a(1) = 1 */
    }

    /* Compute depth d = floor_log2(n)  */
    unsigned d = 0;
    idx_t tmp = n;
    while (tmp >> 1) {
        tmp >>= 1;
        ++d;
    }

    /* offset within the subtree of depth d */
    idx_t offset = n - ((idx_t)1 << d);

    /* First bit check */
    if ((offset >> (d - 1)) != 0) {
        /* Path does not start with the rotated subtree (bit 0) */
        return (seq_t)n;          /* unchanged */
    }

    /* Now we are in the subtree of node 2 (path starting with 0) */
    if (d == 1) {                 /* n == 2 */
        return (seq_t)1 << 2;     /* a(2) = 4  -> new depth 2, offset 0 */
    }

    /* Second bit */
    int bit2 = (offset >> (d - 2)) & 1;
    if (bit2 == 0) {              /* case: 0 0 rest  -> depth +1, prepend 0 */
        idx_t rest = offset & ((1ULL << (d - 2)) - 1);
        unsigned newdepth = d + 1;
        return ((seq_t)1 << newdepth) | (seq_t)rest;
    } else {                      /* bit2 == 1  */
        if (d == 2) {             /* n == 5 */
            return (seq_t)1 << 1; /* a(5) = 2  -> new depth 1, offset 0 */
        }
        int bit3 = (offset >> (d - 3)) & 1;
        if (bit3 == 0) {          /* case: 0 1 0 rest  -> replace 010 by 001 */
            idx_t rest = offset & ((1ULL << (d - 3)) - 1);
            idx_t new_offset = ((idx_t)1 << (d - 3)) | rest;
            return ((seq_t)1 << d) | (seq_t)new_offset;
        } else {                  /* bit3 == 1  -> case: 0 1 1 rest, drop one leading 1 */
            idx_t rest = offset & ((1ULL << (d - 3)) - 1);
            idx_t new_offset = ((idx_t)1 << (d - 3)) | rest;
            unsigned newdepth = d - 1;
            return ((seq_t)1 << newdepth) | (seq_t)new_offset;
        }
    }
}
