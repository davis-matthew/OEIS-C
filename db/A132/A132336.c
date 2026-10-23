/*
 * A132336  Sum of the integers from 1 to n, excluding perfect fifth powers.
 * https://oeis.org/A132336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 - (2*r^6 + 6*r^5 + 5*r^4 - r^2)/12, where r =
 *            floor(n^(1/5))
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   integer_fifth_root, pow5_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The calculations below are performed in unsigned __int128 to avoid
   overflow of intermediate results on the 8‑, 16‑, 32‑, 64‑ and 128‑bit
   builds.  The final value is cast to seq_t, which may be an unsigned
   integer or an arbitrary‑precision type, both of which accept conversion
   from a C integer.  */
#include <stdbool.h>
#include "../utils/integer_fifth_root.h"
#include "../utils/pow5_leq.h"

seq_t a(idx_t n)
{
    idx_t r = integer_fifth_root(n);

    /* Compute the sum of fifth powers up to r using a 128‑bit counter. */
    unsigned __int128 r2 = (unsigned __int128)r * r;
    unsigned __int128 r4 = r2 * r2;
    unsigned __int128 r5 = r4 * r;
    unsigned __int128 r6 = r5 * r;

    unsigned __int128 numerator =
        (unsigned __int128)2 * r6
      + (unsigned __int128)6 * r5
      + (unsigned __int128)5 * r4
      - r2;

    unsigned __int128 sum_powers = numerator / (unsigned __int128)12;

    /* Total sum 1 + 2 + ... + n = n*(n+1)/2 */
    unsigned __int128 total = (unsigned __int128)n * ((unsigned __int128)n + 1) / 2;

    uint64_t diff = (uint64_t)(total - sum_powers);  /* fits in 64 bits */
    return (seq_t)diff;
}
