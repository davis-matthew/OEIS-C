/*
 * A140538  Greatest prime factor of 2*n^4 + 1.
 * https://oeis.org/A140538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = greatest prime factor of 2*n^4 + 1
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 217, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1, but safe guard */

    /* Compute 2*n^4 + 1 in seq_t */
    seq_t x   = (seq_t)n;            /* n */
    seq_t sq  = x * x;               /* n^2 */
    seq_t p4  = sq * sq;             /* n^4 */
    seq_t val = ((seq_t)2 * p4) + 1; /* 2*n^4 + 1 */

    /* For n up to about 10 000, val fits in 64‑bit.  Use the utility
       gpf to get the greatest prime factor. */
    uint64_t v = (uint64_t)val;
    uint64_t g = gpf(v);

    return (seq_t)g;
}
