/*
 * A194848  Write n = C(i,3)+C(j,2)+C(k,1) with i>j>k>=0; sequence gives j
 *          values.
 * https://oeis.org/A194848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal j such that ∃ i>j>k≥0 with n = C(i,3)+C(j,2)+k.
 *            For n = 0 the sequence defines a(0) = 1
 * Range:     u8 n <= 122, u16 n <= 122, u32 n <= 122, u64 n <= 122,
 *            u128 n <= 122, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t binom2(idx_t x) {
    return (x < 2) ? 0 : (x * (x - 1)) / 2;
}

static inline idx_t binom3(idx_t x) {
    return (x < 3) ? 0 : (x * (x - 1) * (x - 2)) / 6;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    for (idx_t j = 0; ; ++j) {
        idx_t bj = binom2(j);
        if (bj > n) break;          /* no further j can work */
        idx_t rem = n - bj;
        for (idx_t i = j + 1; ; ++i) {
            idx_t bi = binom3(i);
            if (bi > rem) break;    /* further i will only increase bi */
            idx_t k = rem - bi;
            if (k < j) {           /* found a valid representation */
                return (seq_t)j;
            }
        }
    }
    /* fall back for the case n = 0 or no representation found */
    return (seq_t)1;
}
