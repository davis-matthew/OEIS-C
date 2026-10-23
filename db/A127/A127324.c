/*
 * A127324  Fourth 4-dimensional hyper-tetrahedral coordinate; 4-D analog of
 *          A056558.
 * https://oeis.org/A127324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the 4th coordinate in the 4‑term combinatorial
 *            representation of n: n = C(i,4) + C(j,3) + C(k,2) + l with i > j
 *            > k > l >= 0. a(n) returns l
 * Range:     u8 n <= 233, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uintptr_t */

/* Helper to compute binomial coefficients for small k via arithmetic,
   using type seq_t for intermediate values. */
static inline seq_t binom4(idx_t x) {
    if (x < 4) return 0;
    return ((seq_t)x * (x-1) * (x-2) * (x-3)) / 24;
}

static inline seq_t binom3(idx_t x) {
    if (x < 3) return 0;
    return ((seq_t)x * (x-1) * (x-2)) / 6;
}

static inline seq_t binom2(idx_t x) {
    if (x < 2) return 0;
    return ((seq_t)x * (x-1)) / 2;
}

seq_t a(idx_t n) {
    /* Find largest i such that C(i,4) <= n. */
    idx_t i = 0;
    while (1) {
        seq_t c4_next = binom4(i + 1);
        if (c4_next > (seq_t)n) break;
        ++i;
    }
    seq_t rem = (seq_t)n - binom4(i);

    /* Find largest j < i such that C(j,3) <= rem. */
    idx_t j = 0;
    while (1) {
        if (j + 1 >= i) break;
        seq_t c3_next = binom3(j + 1);
        if (c3_next > rem) break;
        ++j;
    }
    seq_t rem2 = rem - binom3(j);

    /* Find largest k < j such that C(k,2) <= rem2. */
    idx_t k = 0;
    while (1) {
        if (k + 1 >= j) break;
        seq_t c2_next = binom2(k + 1);
        if (c2_next > rem2) break;
        ++k;
    }
    seq_t l = rem2 - binom2(k);
    return l;
}
