/*
 * A062741  3 times pentagonal numbers: 3*n*(3*n-1)/2.
 * https://oeis.org/A062741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n*n - 3*n)/2 (3 times pentagonal numbers)
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the closed‑form formula and a 128‑bit
   intermediate to avoid overflow on 64‑bit builds.  It works for any
   index n ≥ 0 and requires no memoisation. */
#include <stdint.h>
#include <inttypes.h>

seq_t a(idx_t n)
{
    /* Use a 128‑bit integer for the intermediate calculation to avoid
       overflow on 64‑bit seq_t builds.  On arbitrary‑precision builds
       the cast has no effect because seq_t can accommodate the value. */
    unsigned __int128 temp = (unsigned __int128)9 * (unsigned __int128)n * (unsigned __int128)n;
    temp -= (unsigned __int128)3 * (unsigned __int128)n;
    temp /= 2;
    return (seq_t)temp;
}
