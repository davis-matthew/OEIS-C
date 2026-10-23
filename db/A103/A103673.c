/*
 * A103673  If in binary representation n! contains 5! then 1 else 0.
 * https://oeis.org/A103673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   msb_position
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Checks whether the binary representation of n! contains the binary
   representation of 5! (=1111000).  a(n) = 1 if it does, 0 otherwise.
   For n > 65 the conjecture guarantees that the pattern always occurs,
   so the function simply returns 1. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/msb_position.h"

static const unsigned int PATTERN = 0b1111000;   /* 5! = 120 in binary */
static const unsigned int PATLEN   = 7;
static const unsigned int MASK     = (1U << PATLEN) - 1;
static const unsigned int MAX_N   = 65;          /* explicit factorial range */

/* a(n) as defined in OEIS A103673 */
seq_t a(idx_t n)
{
    /* For n > 65 the conjecture guarantees a(n) = 1 */
    if (n > MAX_N) return (seq_t)1;

    /* Compute n! using 32‑bit limbs (little‑endian).  For n ≤ 65 only
       a few limbs are required (≤10). */
    uint32_t limbs[20];
    size_t count = 1;
    limbs[0] = 1;                     /* start with 1! = 1 */

    for (uint32_t i = 2; i <= (uint32_t)n; ++i) {
        unsigned long long carry = 0;
        for (size_t k = 0; k < count; ++k) {
            unsigned long long prod = (unsigned long long)limbs[k] * i + carry;
            limbs[k] = (uint32_t)prod;
            carry   = prod >> 32;
        }
        if (carry) {
            limbs[count++] = (uint32_t)carry;
        }
    }

    /* Find number of bits in the factorial */
    size_t ms_index = count - 1;
    while (ms_index > 0 && limbs[ms_index] == 0) --ms_index;
    int msb_in_limb = msb_position(limbs[ms_index]);
    int total_bits  = (int)(ms_index * 32 + msb_in_limb + 1);

    /* Scan bits from most significant to least significant,
       keeping a sliding window of the last PATLEN bits. */
    unsigned int window = 0;
    int bits_seen = 0;

    for (int bit_pos = total_bits - 1; bit_pos >= 0; --bit_pos) {
        size_t limb_idx = (size_t)bit_pos >> 5;          /* bit_pos / 32 */
        int   bit_idx   = bit_pos & 31;                 /* bit_pos % 32 */
        int   bit = (limbs[limb_idx] >> bit_idx) & 1;

        window = ((window << 1) | bit) & MASK;
        ++bits_seen;
        if (bits_seen >= PATLEN && window == PATTERN) return (seq_t)1;
    }

    return (seq_t)0;
}
