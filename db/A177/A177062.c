/*
 * A177062  Length of the longest common substring in binary representations
 *          of n and n^2.
 * https://oeis.org/A177062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = length of longest common substring of the binary *
 *            representations of n and n^2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   extract_bits_uint128, extract_bits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/extract_bits_uint128.h"
#include "../utils/extract_bits_uint64.h"

seq_t a(idx_t n)
{
    /* handle offsets safely */
    if (n == 0) return (seq_t)0;

    /* binary bits of n */
    unsigned char bits1[128];
    int len1 = extract_bits_uint64((uint64_t)n, bits1);

    /* binary bits of n^2 using 128‑bit for safety */
    __uint128_t sq = (__uint128_t)n * n;
    unsigned char bits2[128];
    int len2 = extract_bits_uint128(sq, bits2);

    /* dynamic programming for longest common substring */
    int prev[129] = {0};      /* length len2+1, initialized to 0 */
    int cur[129]  = {0};      /* scratch buffer */
    unsigned int max_len = 0;

    for (int i = 0; i < len1; ++i) {
        for (int j = 0; j < len2; ++j) {
            if (bits1[i] == bits2[j]) {
                cur[j + 1] = prev[j] + 1;
                if ((unsigned int)cur[j + 1] > max_len)
                    max_len = (unsigned int)cur[j + 1];
            } else {
                cur[j + 1] = 0;
            }
        }
        /* swap prev and cur, and clear cur for next iteration */
        memcpy(prev, cur, (len2 + 1) * sizeof(int));
        memset(cur, 0, (len2 + 1) * sizeof(int));
    }
    return (seq_t)max_len;
}
