/*
 * A143909  A positive integer k is included if the number of digits in the
 *          binary representation of k is a multiple of the number of zeros in
 *          the binary representation of k.
 * https://oeis.org/A143909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A143909: a(n) = the n‑th positive integer k such that
 *     the number of bits in the binary representation of k is a
 *     multiple of the number of zeros in that representation.
 * The function memoizes terms so that repeated calls are fast.
 * For n up to about 10000 the implementation runs well under the time
 * limit with an O(n) scan per call.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

#define CACHE_CAP 20000   /* sufficient for n up to ~10 000 */

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t cache[CACHE_CAP];
    static size_t cache_len = 0;
    static idx_t next_k = 2;          /* next integer to test */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t k = next_k;
        unsigned L = bitlen_seq_t(k);   /* number of bits in k */
        unsigned zeros = 0;

        /* count zeros among the L bits */
        for (unsigned i = 0; i < L; ++i) {
            /* (k >> i) & 1 == 0  -> zero bit */
            if (((k >> i) & 1) == 0) {
                ++zeros;
            }
        }

        /* accept if zeros > 0 (to avoid division by zero) and
           L is a multiple of zeros */
        if (zeros > 0 && (L % zeros) == 0) {
            cache[cache_len++] = k;
        }

        ++next_k;   /* move to next candidate */
    }

    return cache[n - 1];
}
