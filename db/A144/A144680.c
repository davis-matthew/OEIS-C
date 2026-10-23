/*
 * A144680  Triangle read by rows, lower half of an array formed by A004736 *
 *          A144328 (transform).
 * https://oeis.org/A144680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the smallest integer with r(r+1)/2 ≥ n
 *            * and k = n − r(r−1)/2. The triangle entry is * T(r,k) = (1/6) *
 *            ( 3*(k² − k + 2)*r − k*(k−1)*(2k−1) ). * This implements the
 *            closed‑form formula given in the OEIS entry
 * Range:     u8 n <= 25, u16 n <= 939, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t _row_from_index(uint64_t idx) {
    /* Find minimal r such that r(r+1)/2 ≥ idx */
    uint64_t lo = 1, hi = idx;
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= idx)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t idx) {
    /* Minimum valid index is 1 */
    if (idx == 0)
        return (seq_t)0;

    uint64_t r = _row_from_index(idx);
    uint64_t prev_tri = (uint64_t)((unsigned __int128)(r - 1) * r / 2);
    uint64_t k = idx - prev_tri;

    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;

    seq_t ksq = sk * sk;                                   /* k² */
    seq_t temp = ksq - sk + (seq_t)2;                      /* k² − k + 2 */
    seq_t part1 = temp * sr * (seq_t)3;                    /* 3*(k² − k + 2)*r */

    seq_t part2 = sk * ((seq_t)k - (seq_t)1);              /* k*(k−1) */
    part2 = part2 * ((seq_t)2 * sk - (seq_t)1);           /* *(2k−1) */

    seq_t numerator = part1 - part2;
    seq_t result = numerator / (seq_t)6;                  /* divide by 6 */

    return result;
}
