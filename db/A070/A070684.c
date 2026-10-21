/*
 * A070684  n^6 mod 44.
 * https://oeis.org/A070684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 44 (offset 0)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce the base modulo 44 to keep the intermediate values small. */
    seq_t m = (seq_t)n % 44U;       /* base modulo 44 */

    /* Compute m^6 mod 44 iteratively, using a 64‑bit intermediate to avoid
       overflow on small seq_t types. */
    seq_t r = 1;
    for (idx_t i = 0; i < 6; ++i)
    {
        uint64_t prod = (uint64_t)r * (uint64_t)m;  /* safe intermediate */
        r = (seq_t)(prod % 44U);
    }
    return r;
}
