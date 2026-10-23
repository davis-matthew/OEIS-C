/*
 * A184004  a(n) = n + floor(sqrt(4n/3)); complement of A184005.
 * https://oeis.org/A184004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(4n/3))
 * Range:     u8 n <= 47, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS definition uses off‑by‑one indexing: a(1) is the first term. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to the sequence element type to avoid
       overflow when computing 4*n.  The size of seq_t depends on
       the build (uint8_t … uint128).  Using seq_t for all intermediate
       values keeps the calculation exact. */
    seq_t sn = (seq_t)n;          /* n as seq_t */
    seq_t val = sn * 4;           /* 4 * n */
    seq_t div = val / 3;          /* floor(4n/3) */
    seq_t sq  = floor_sqrt(div);  /* floor(sqrt(floor(4n/3))) */
    return sn + sq;               /* n + floor(sqrt(4n/3)) */
}
