/*
 * A136137  a(n+1)=sopfr(3a(n)+1), with sopfr=A001414. Finishes with 17 (fixed
 *          point).
 * https://oeis.org/A136137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n+1) = sopfr(3*a(n)+1) with a(1)=1 sopfr(n) = sum of prime
 *            factors of n with multiplicity (OEIS A001414)
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   _sopfr_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_sopfr_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined, but avoid wrap‑around */
        return 0;

    seq_t current = 1;      /* a(1) */

    for (idx_t i = 1; i < n; ++i) {   /* compute up to a(n) */
        /* 3*a + 1 should fit in uint64_t for this sequence */
        uint64_t val63 = (uint64_t)current;
        uint64_t next_val = 3ULL * val63 + 1ULL;

        current = (seq_t)_sopfr_uint64(next_val);
    }

    return current;
}
