/*
 * A018589  Divisors of 680.
 * https://oeis.org/A018589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 680 in ascending order (if n > 16, returns
 *            0)
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   _cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_cmp_seq_t.h"

static int _divisors_initialized = 0;
static seq_t _divisors[16] = {0};

seq_t a(idx_t n)
{
    /* initialise the divisor list once */
    if (!_divisors_initialized) {
        size_t idx = 0;
        for (int a = 0; a <= 3; ++a) {
            seq_t pow2 = ((seq_t)1) << a;
            for (int b = 0; b <= 1; ++b) {
                seq_t pow5 = b ? (seq_t)5 : (seq_t)1;
                for (int c = 0; c <= 1; ++c) {
                    seq_t pow17 = c ? (seq_t)17 : (seq_t)1;
                    _divisors[idx++] = pow2 * pow5 * pow17;
                }
            }
        }
        qsort(_divisors, 16, sizeof(seq_t), _cmp_seq_t);
        _divisors_initialized = 1;
    }

    if (n <= 0)
        return (seq_t)0;
    if (n > 16)
        return (seq_t)0;
    return _divisors[(size_t)(n - 1)];
}
