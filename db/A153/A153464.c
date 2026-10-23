/*
 * A153464  Numbers k such that 4*k+9 is not prime.
 * https://oeis.org/A153464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _fits_uint64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 4*k+9 is not prime.
   a(n) is the n-th integer k (1‑based) for which 4*k+9 is composite. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"   /* bool is_prime_small(uint64_t) */
#include "../utils/_fits_uint64.h"

/* Return the n-th k such that 4*k+9 is not prime.  The index n is 1‑based.
 * No memoisation is needed; each call recomputes from scratch, but the
 * cost is very small for n up to about 10,000. */
seq_t a(idx_t n)
{
    if (n == 0)        /* offset is 1; undefined behaviour for n < 1 */
        return 0;

    seq_t k = 0;               /* candidate k */
    idx_t count = 0;           /* how many composite 4*k+9 found so far */

    while (count < n) {
        seq_t val = 4 * k + 9;          /* 4*k+9 as seq_t */
        if (! _fits_uint64(val))       /* should not happen for test ranges */
            return k;                  /* fall back to current k */

        if (! is_prime_small((uint64_t)val)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
    return k;   /* unreachable, keeps compiler happy */
}
