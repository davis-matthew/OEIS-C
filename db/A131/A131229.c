/*
 * A131229  Numbers congruent to {1,7} mod 10.
 * https://oeis.org/A131229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n - 3 (n even), a(n) = 5*n - 4 (n odd), for n ≥ 1
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Perform multiplication in 64‑bit to avoid overflow before
       conversion to seq_t.  The cast to seq_t is safe for all
       supported seq_t types (unsigned integers or arbitrary precision). */
    uint64_t tmp = (uint64_t)5 * n;
    seq_t res = (seq_t)tmp;

    /* Subtract the appropriate constant according to parity of n. */
    if ((n & 1) == 0)
        res -= 3;  /* even n */
    else
        res -= 4;  /* odd n */

    return res;
}
