/*
 * A175045  a(n) = number of distinct values k of substrings in binary n where
 *          both k is prime and A030101(k) is prime.
 * https://oeis.org/A175045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of distinct substring values k of the binary *
 *            representation of n such that k is prime and A030101(k) (the
 *            decimal * value of the binary digits of k reversed) is also
 *            prime
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"

#define MAX_SUBS 2100   /* > 64*65/2 = 2080, maximum number of substrings for 64‑bit n */

/* External linkage function required by the test harness */
seq_t a(idx_t n)
{
    /* n = 0 has only the substring "0", which is not prime */
    if (n == 0)
        return 0;

    /* Extract the binary representation of n into an array of bits.
     * bits[0] will be the least‑significant bit. After reversal we
     * obtain msb[0] as the most‑significant bit. */
    idx_t tmp = n;
    idx_t bits[64];
    idx_t len = 0;
    while (tmp) {
        bits[len++] = tmp & 1u;
        tmp >>= 1u;
    }

    idx_t msb[64];
    for (idx_t i = 0; i < len; ++i)
        msb[i] = bits[len - 1 - i];   /* most‑significant first */

    /* Keep track of the distinct substring values that have already
     * been examined.  A small static array is sufficient because the
     * maximum number of substrings for a 64‑bit integer is 2080. */
    static seq_t seen[MAX_SUBS];
    size_t seen_count = 0;
    seq_t count = 0;

    /* Enumerate all contiguous substrings of msb[0..len-1]. */
    for (idx_t i = 0; i < len; ++i) {
        seq_t val = 0;
        for (idx_t j = i; j < len; ++j) {
            val = (val << 1) | msb[j];

            /* Skip if this value has already been counted. */
            bool duplicate = false;
            for (size_t k = 0; k < seen_count; ++k) {
                if (seen[k] == val) {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate)
                continue;

            seen[seen_count++] = val;

            /* Check whether both val and its binary reversal are prime. */
            if (!is_prime_small(val))
                continue;

            /* Compute the reversal of the binary digits of val. */
            seq_t rev = 0;
            seq_t t = val;
            while (t > 0) {
                rev = (rev << 1) | (t & 1);
                t >>= 1;
            }

            if (is_prime_small(rev))
                count = count + 1;
        }
    }
    return count;
}
