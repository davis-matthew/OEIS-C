/*
 * A059012  Numbers that have an even number of 0's and 1's in their binary
 *          expansion.
 * https://oeis.org/A059012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th number whose binary representation has an even
 *            number of 0's and an even number of 1's (hence even bit-length
 *            and even popcount)
 * Range:     u8 n <= 85, u16 n <= 21845, u32 n <= 32768, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 73 terms through n = 9805 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: number of bits in the binary expansion (no leading zeros). */
static inline unsigned bitlen(uint64_t v) {
    return 64 - __builtin_clzll(v);
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    uint64_t x = 1;          /* start from 1 to skip 0 */
    while (1) {
        unsigned len = bitlen(x);
        if ((len & 1) == 0) {                      /* even bit-length */
            unsigned pop = __builtin_popcountll(x);
            if ((pop & 1) == 0) {                  /* even popcount */
                cnt++;
                if (cnt == n) {
                    return (seq_t)x;                /* cast to requested type */
                }
            }
        }
        ++x;
    }
}
