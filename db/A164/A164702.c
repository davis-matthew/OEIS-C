/*
 * A164702  Write n in binary. Insert one 0 right of any one 1. a(n) is the
 *          smallest possible composite equal to the value of any such
 *          resulting binary number.
 * https://oeis.org/A164702
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest composite number obtained by inserting a
 *            single 0 immediately to the right of any 1 in the binary
 *            representation of n
 * Range:     u8 n <= 127, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 85 terms through n = 9622 (2026-11-01)
 * Helpers:   is_prime_seq, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/pow2.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* undefined for n<2, return 0 */

    seq_t best = 0;
    bool found = false;

    for (idx_t i = 0; (n >> i); ++i) {
        if (!((n >> i) & 1)) continue;   /* skip positions without a 1 */

        seq_t high = (seq_t)(n >> (i + 1));
        seq_t low  = (seq_t)(n & (((idx_t)1 << i) - 1));

        /* m = high * 2^(i+2) + 2^(i+1) + low */
        seq_t m = high * pow2(i + 2) + pow2(i + 1) + low;

        if (!is_prime_seq(m)) {          /* composite candidate */
            if (!found || m < best) {
                best = m;
                found = true;
            }
        }
    }
    return best;
}
