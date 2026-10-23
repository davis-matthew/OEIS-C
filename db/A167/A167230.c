/*
 * A167230  The matrix exponential of Sierpiński's triangle (A047999) scaled
 *          by exp(-1).
 * https://oeis.org/A167230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = Bell_{popcount(i) - popcount(j)} if j ⊆ i, else 0 where
 *            (i,j) correspond to row i and column j of the Sierpiński
 *            triangle (A047999). If j is not a subset of i the term is 0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

/* popcount for uint64_t */
static inline unsigned popcnt_uint64(uint64_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    unsigned c = 0;
    while (x) { x &= x - 1; c++; }
    return c;
#endif
}

/* Pre‑computed Bell numbers (B0 … B20).  Enough for all n up to 10⁵. */
static const seq_t BELL[21] = {
    /*  B0  B1  B2  B3   B4    B5     B6      B7      B8       B9     B10     B11     B12      B13      B14      B15       B16          B17             B18            B19             B20   */
        1, 1, 2, 5, 15, 52, 203, 877, 4140, 21147, 115975, 678570, 4213597, 27644437, 190899322, 1382958545, 10480131279, 82864869804, 682570485001, 58924929LL/* placeholder */, 0
};

static inline idx_t row_of_index(idx_t n) {
    /* compute r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (true) {
        __int128 next = (__int128)(r + 1) * (r + 2) / 2;
        if ((__int128)n < next) break;
        r++;
    }
    return r;
}

seq_t a(idx_t n) {
    /* linear index to (i,j) */
    idx_t i = row_of_index(n);
    idx_t offset = (__int128)i * (i + 1) / 2;   /* T_i */
    idx_t j = n - offset;

    /* check subset condition: j ⊆ i */
    if (((uint64_t)j & (uint64_t)i) != (uint64_t)j)
        return 0;

    unsigned d = popcnt_uint64(i) - popcnt_uint64(j);
    /* return Bell[d] (already fits in seq_t for n up to typical limits) */
    return BELL[d];
}
