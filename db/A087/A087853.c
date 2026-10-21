/*
 * A087853  Erroneous generalization of A087851.
 * https://oeis.org/A087853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = ⌊n*φ⌋ (for n ≥ 8, the first 7 terms are erroneous and are
 *            hard‑coded as listed in the OEIS entry)
 * Range:     i8 n <= 26, i16 n <= 26, i32 n <= 26, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for uint64_t, returns ⌊√v⌋. */
static inline uint64_t int_sqrt_u64(uint64_t v)
{
    uint64_t low = 0, high = v;
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t sq = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;   /* floor of the square root */
}

seq_t a(idx_t n)
{
    /* First seven terms are part of the erroneous generalization. */
    static const seq_t small[8] = {0, 1, 1, 3, 3, 5, 4, 8};

    if (n < 8) {
        return small[(idx_t)n];
    }

    /* Compute floor(n*φ) exactly via integer arithmetic: */
    /* floor(n*φ) = ⌊(n + ⌊n*√5⌋)/2⌋  with  ⌊n*√5⌋ = ⌊√(5*n²)⌋. */
    __int128 prod = (__int128)5 * n * n;          /* 5*n²   (fits in 128 bits) */
    uint64_t prod64 = (uint64_t)prod;            /* value ≤ 5·10⁸ for n≤10⁴ */
    uint64_t sq = int_sqrt_u64(prod64);          /* ⌊n*√5⌋  */
    seq_t res = (seq_t)n + (seq_t)sq;            /* n + ⌊n*√5⌋ */
    res /= 2;                                    /* floor division by 2 */
    return res;
}
