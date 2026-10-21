/*
 * A080542  In binary representation: keep the first digit and rotate right
 *          the others.
 * https://oeis.org/A080542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) keeps the most‑significant binary digit of n * in place and
 *            rotates the remaining bits one position to the right. * In
 *            closed form: a(n) = 2^⌊log₂n⌋ + ⌊(n-2^⌊log₂n⌋)/2⌋ + (n mod
 *            2)·2^(⌊log₂n⌋-1)
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute ⌊log₂x⌋ for x>0 using only integer shifts */
static inline uint64_t floor_log2_uint64(uint64_t x) {
    uint64_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

seq_t a(idx_t n) {
    /* a(1) = 1 */
    if (n <= 1) return (seq_t)1;

    /* Compute the position of the most significant bit (0‑based) */
    uint64_t k = floor_log2_uint64((uint64_t)n);      /* 1 <= k <= 63 */
    uint64_t msb = (uint64_t)1 << k;                  /* 2^k */
    uint64_t rest = (uint64_t)n - msb;                /* bits below msb */
    uint64_t low  = rest & 1ULL;                      /* original lsb */
    uint64_t rotated_rest = rest >> 1;                /* shift right by 1 */
    uint64_t result = msb | (low << (k - 1)) | rotated_rest;
    return (seq_t)result;
}
