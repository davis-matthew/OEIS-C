/*
 * A100290  Numbers divisible by smallest number with same number of 1's in
 *          its binary expansion. That is, A038573(a(n)) divides a(n).
 * https://oeis.org/A100290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 1933, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   smallest_with_k_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/smallest_with_k_bits.h"

/* Sequence A100290: a(n) is the nth positive integer k such that
   k is divisible by the smallest integer whose binary representation
   has the same number of 1s as k. */

/* Count 1-bits of a seq_t value. */
static unsigned int popcount_seq(seq_t x)
{
#if defined(__SIZEOF_INT128__) && defined(UINT128_MAX)
    /* If the platform supports 128-bit integers, handle the high half
       for seq_t larger than 64 bits. */
    if (sizeof(seq_t) <= 8) {
        return __builtin_popcountll((uint64_t)x);
    } else {
        unsigned int low  = __builtin_popcountll((uint64_t)x);
        unsigned int high = __builtin_popcountll(
            (uint64_t)((unsigned __int128)x >> 64));
        return low + high;
    }
#else
    /* Assume seq_t fits in 64 bits. */
    return __builtin_popcountll((uint64_t)x);
#endif
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* Undefined for n<1, return 0 */

    idx_t count = 0;
    seq_t cur   = 1;                        /* Start from the first positive integer */

    for (;;) {
        unsigned int k = popcount_seq(cur);
        seq_t s = smallest_with_k_bits(k);
        if (s != 0 && cur % s == 0) {
            ++count;
            if (count == n) return cur;
        }
        ++cur;
    }
}
