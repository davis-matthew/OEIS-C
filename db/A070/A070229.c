/*
 * A070229  Next m>n such that m is divisible by gpf(n), gpf=A006530 (greatest
 *          prime factor).
 * https://oeis.org/A070229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + gpf(n) (greatest prime factor of n)
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _gpf_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special case: a(1) = 2 */

#include <stdint.h>
#include "../utils/_gpf_uint64.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;
    if (n == 0) return (seq_t)0;       /* behaviour for n < 1 is unspecified */

    seq_t gpf = (seq_t)_gpf_uint64((uint64_t)n);
    return (seq_t)n + gpf;
}
