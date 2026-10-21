/*
 * A088885  Minimum number of consecutive previous nonnegative integers to n
 *          that must be concatenated together in descending order such that n
 *          divides the concatenated term, or zero if n divides no such
 *          concatenation.
 * https://oeis.org/A088885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest k ≥ 1 such that the concatenation of n-1,
 *            n-2, …, n-k is divisible by n. If no such k exists, a(n) = 0.
 *            (e.g. a(8)=6 because 765432 is 0 mod 8; a(7)=0 because no such
 *            concatenation exists.)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   digits_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digits_idx_t.h"

seq_t a(idx_t n)
{
    if (n == 0) /* below the offset – undefined, return 0 */
        return (seq_t)0;

    uint64_t mod_n = (uint64_t)n;

    /* start with the single-term concatenation: just n-1 */
    uint64_t val = (uint64_t)(n - 1) % mod_n;
    if (val == 0)                     /* k = 1 */
        return (seq_t)1;

    /* iterate adding one more previous term at a time */
    for (idx_t i = n - 2; ; --i) {
        unsigned d = digits_idx_t(i);

        /* compute 10^d mod n */
        uint64_t pow10 = 1;
        for (unsigned j = 0; j < d; ++j)
            pow10 = (pow10 * 10) % mod_n;

        /* update the current concatenated value modulo n */
        val = (val * pow10 + i) % mod_n;

        if (val == 0) {                 /* found minimal k */
            idx_t k = (n - 1) - i + 1;   /* number of terms used */
            return (seq_t)k;
        }

        if (i == 0)                    /* reached the lower bound */
            break;
    }

    return (seq_t)0;                     /* no suitable concatenation */
}
