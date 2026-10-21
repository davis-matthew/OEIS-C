/*
 * A214561  Number of 1's in binary expansion of n^n.
 * https://oeis.org/A214561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 1's in the binary representation of n^n
 * Range:     u8 n <= 78, u16 n <= 9836, u32 n <= 9776, u64 n <= 9776,
 *            u128 n <= 9776, big any n
 * Helpers:   mul_small_limb
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A214561) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mul_small_limb.h"

seq_t a(idx_t n)
{
    /* 0^0 is defined as 1 in this sequence. */
    if (n == 0)
        return (seq_t)1;

    /* Dynamic array of 64‑bit limbs, little‑endian. */
    size_t cap  = 16;          /* initial capacity */
    size_t sz   = 1;           /* current number of limbs */
    uint64_t *limb = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!limb) return (seq_t)0;      /* out of memory – return 0 as sentinel */

    limb[0] = 1;               /* start with 1 */

    /* Repeatedly multiply by n, n times: compute n^n. */
    for (idx_t i = 0; i < n; ++i) {
        mul_small_limb(limb, &sz, &cap, (uint64_t)n);
    }

    /* Count 1‑bits across all limbs. */
    idx_t ones = 0;
    for (size_t i = 0; i < sz; ++i)
        ones += __builtin_popcountll(limb[i]);

    free(limb);
    return (seq_t)ones;
}
