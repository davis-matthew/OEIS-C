/*
 * A205509  Hamming distance between (n-1)! and n!.
 * https://oeis.org/A205509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Hamming distance between (n-1)! and n! (in binary)
 * Range:     u8 n <= 113, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

/* Maximum number of 32‑bit limbs needed for 10000! :
   bits ≈ n log2 n – n ≈ 1.328×10^5, limbs ≈ 4137.          */
#define MAX_LIMBS 8192

/* Popcount for 32‑bit words, portable. */
static unsigned popcount32(uint32_t x) {
    x -= (x >> 1) & 0x55555555u;
    x = (x & 0x33333333u) + ((x >> 2) & 0x33333333u);
    return (((x + (x >> 4)) & 0x0F0F0F0Fu) * 0x01010101u) >> 24;
}

/* Multiply a big integer given by limbs and length by a small unsigned
   integer m (m ≤ 10000). The limbs are stored least‑significant first. */
static void mul_small(uint32_t *limbs, size_t *len, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint64_t prod = (uint64_t)limbs[i] * m + carry;
        limbs[i]     = (uint32_t)(prod & 0xFFFFFFFFu);
        carry        = prod >> 32;
    }
    while (carry) {
        limbs[*len] = (uint32_t)(carry & 0xFFFFFFFFu);
        carry      >>= 32;
        ++(*len);
    }
}

seq_t a(idx_t n) {
    /* Represent factorials as little‑endian arrays of 32‑bit limbs. */
    static uint32_t prev_limbs[MAX_LIMBS];
    static uint32_t curr_limbs[MAX_LIMBS];
    size_t prev_len, curr_len;
    uint64_t pop;      /* count fits in 64 bits for all n ≤ 10000 */

    if (n < 1) return (seq_t)0;   /* offset is 1, but guard for safety */

    /* (n-1)! starts at 1 */
    prev_limbs[0] = 1;
    prev_len = 1;
    for (idx_t i = 1; i < n; ++i) {
        mul_small(prev_limbs, &prev_len, (uint32_t)i);
    }

    /* n! = (n-1)! * n */
    memcpy(curr_limbs, prev_limbs, prev_len * sizeof(uint32_t));
    curr_len = prev_len;
    mul_small(curr_limbs, &curr_len, (uint32_t)n);

    /* XOR the two factorials and pop their bits. */
    size_t maxlen = prev_len > curr_len ? prev_len : curr_len;
    pop = 0;
    for (size_t i = 0; i < maxlen; ++i) {
        uint32_t a = (i < prev_len ? prev_limbs[i] : 0);
        uint32_t b = (i < curr_len ? curr_limbs[i] : 0);
        pop += popcount32(a ^ b);
    }

    return (seq_t)pop;
}
