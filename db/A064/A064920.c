/*
 * A064920  a(n) = n/gpf(n) + gpf(n) - 1, where gpf = A006530 = greatest prime
 *          factor.
 * https://oeis.org/A064920
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = n/gpf(n) + gpf(n) - 1
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gpf_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* gpf(n) is the greatest prime factor of n. */

#include <stdint.h>
#include "../utils/gpf_u64.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* behavior below the offset is undefined */

    uint64_t n64 = (uint64_t)n;
    uint64_t gpf64 = gpf_u64(n64);

    seq_t nseq = (seq_t)n64;
    seq_t gseq = (seq_t)gpf64;

    return nseq / gseq + gseq - 1;
}
