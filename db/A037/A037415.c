/*
 * A037415  Positive numbers having the same set of digits in base 2 and base
 *          10.
 * https://oeis.org/A037415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose decimal representation
 *            consists * of only the digits 0 and 1, excluding the all-ones
 *            pattern beyond the * single-digit 1
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 1014, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t len = 1; ; ++len) {          /* current string length */
        idx_t max_mask = 1ULL << (len - 1);   /* 2^(len-1) possible suffixes */
        for (idx_t mask = 0; mask < max_mask; ++mask) {
            if (len > 1 && mask == max_mask - 1) continue; /* skip all‑ones */
            uint64_t val = 1;                 /* leading '1' digit */
            for (idx_t i = 0; i < len - 1; ++i) {
                uint64_t bit = (mask >> (len - 2 - i)) & 1ULL;
                val = val * 10 + bit;
            }
            ++count;
            if (count == n) return (seq_t)val;
        }
    }
}
