/*
 * A066486  a(n) = min( x : x^3 + n^3 == 0 mod (x+n-1) ).
 * https://oeis.org/A066486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min { d - n + 1 : d | (3n^2 - 3n + 1) , d > n-1 }
 * Range:     u8 n <= 5, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here d = x + n - 1 is a positive divisor of 3n^2 - 3n + 1 larger than n - 1. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* for floor_sqrt(seq_t) */

/* Compute a(n) for n >= 1.  */
seq_t a(idx_t n) {
    /* Compute N = 3n^2 - 3n + 1 in seq_t for exactness. */
    seq_t sn = (seq_t)n;
    seq_t f = (seq_t)3 * sn * sn - (seq_t)3 * sn + (seq_t)1;

    /* Find the smallest divisor d of f that is > n-1. */
    /* sqrt_f fits into 64-bit for n <= 10000; cast safely. */
    seq_t sqrt_f_seq = floor_sqrt(f);
    uint64_t sqrt_f = (uint64_t)sqrt_f_seq;   /* sth <= ~ 17320 for n <= 10000 */

    seq_t min_d = f;   /* f itself always satisfies d > n-1 for n>=1 */
    seq_t n_minus_1 = sn - (seq_t)1;

    for (uint64_t i = 1; i <= sqrt_f; ++i) {
        seq_t divisor = (seq_t)i;
        if (f % divisor == (seq_t)0) {
            seq_t pair = f / divisor;   /* divisor * pair == f */

            /* Check the small divisor */
            if (divisor > n_minus_1 && divisor < min_d) {
                min_d = divisor;
            }
            /* Check the paired divisor */
            if (pair > n_minus_1 && pair < min_d) {
                min_d = pair;
            }
        }
    }

    /* a(n) = d - n + 1 */
    return min_d - sn + (seq_t)1;
}
