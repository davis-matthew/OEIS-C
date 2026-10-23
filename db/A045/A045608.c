/*
 * A045608  Numbers k that divide 10^k + 8^k.
 * https://oeis.org/A045608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer k such that k divides 10^k +
 *            8^k
 * Range:     u8 n <= 19, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset starts at 1 */

    idx_t count = 0;
    idx_t k = 1;

    for (;;) {
        seq_t t1 = mod_pow((seq_t)10, k, (seq_t)k);
        seq_t t2 = mod_pow((seq_t)8,  k, (seq_t)k);

        /* (10^k + 8^k) mod k == 0? */
        if ( (t1 + t2) % (seq_t)k == (seq_t)0 ) {
            count++;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
