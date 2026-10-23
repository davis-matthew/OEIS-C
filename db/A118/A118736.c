/*
 * A118736  Number of zeros in binary expansion of 3^n.
 * https://oeis.org/A118736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   Number of zeros in the binary expansion of 3^n a(n) =
 *            bitlen(3^n) – popcount(3^n) where bitlen(x) = floor(log2(x)) + 1
 * Range:     u8 n <= 313, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: count set bits in a 64‑bit word.
   __builtin_popcountll is available in C11/C++17. */
static inline unsigned long long popcount64(uint64_t w)
{
    return __builtin_popcountll(w);
}

/* Helper: count bits in a 64‑bit word.
   __builtin_clzll is defined for non‑zero values. */
static inline unsigned int bitlen64(uint64_t w)
{
    return 64 - __builtin_clzll(w);
}

/* Main sequence function.  It works for any seq_t and any idx_t.
   For seq_t of arbitrary‑precision types the implementation
   uses its underlying type for the final count.  The intermediate
   arithmetic is performed on 64‑bit limbs which are small enough
   for any n ≤ 10000. */
seq_t a(idx_t n)
{
    /* Allocate a buffer capable of holding up to ~16000 bits
       (~250 64‑bit limbs).  4096 limbs provide ample safety. */
    uint64_t limbs[4096];
    size_t limb_cnt = 1;      /* start with value 1 */
    limbs[0] = 1ULL;

    /* Compute 3^n by repeated multiplication on the limb array. */
    for (idx_t i = 0; i < n; ++i) {
        unsigned __int128 carry = 0;
        for (size_t j = 0; j < limb_cnt; ++j) {
            unsigned __int128 prod = (unsigned __int128)limbs[j] * 3 + carry;
            limbs[j] = (uint64_t)prod;       /* low 64 bits */
            carry   = prod >> 64;            /* high 64 bits as carry */
        }
        if (carry != 0) {
            limbs[limb_cnt++] = (uint64_t)carry;
        }
    }

    /* Count set bits and determine the bit length of 3^n. */
    unsigned long long pop = 0;
    for (size_t j = 0; j < limb_cnt; ++j) {
        pop += popcount64(limbs[j]);
    }

    /* The most significant limb is limbs[limb_cnt - 1]. */
    unsigned int msb = bitlen64(limbs[limb_cnt - 1]); /* number of bits in this limb */
    size_t bitlen = (limb_cnt - 1) * 64 + msb;

    /* Number of zeros = total bits – number of ones. */
    seq_t zeros = (seq_t)(bitlen - pop);
    return zeros;
}
