/*
 * A111025  Number of cubes between 10n and 10n+9 (inclusive).
 * https://oeis.org/A111025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ m ≥ 0 : 10*n ≤ m^3 ≤ 10*n+9 }
 * Range:     u8 n <= 20, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   cube, cube_root_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/cube.h"
#include "../utils/cube_root_floor.h"

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    seq_t ten_n = nseq * (seq_t)10;
    seq_t ten_n_plus_9 = ten_n + (seq_t)9;

    seq_t lo = cube_root_floor(ten_n);
    if (cube(lo) < ten_n)
        lo = lo + (seq_t)1;

    seq_t hi = cube_root_floor(ten_n_plus_9);

    if (hi >= lo)
        return hi - lo + (seq_t)1;
    else
        return (seq_t)0;
}
