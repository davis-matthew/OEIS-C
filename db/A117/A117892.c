/*
 * A117892  Add up the positive integers which are coprime to n in order
 *          (starting at 1). a(n) is the largest such partial sum that is <=
 *          n.
 * https://oeis.org/A117892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = largest partial sum of positive integers coprime to n that does not exceed n */
#include <stdint.h>

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safe fallback for out‑of‑range indices */
    seq_t sum = 0;
    for (idx_t i = 1; ; ++i) {
        if (gcd_idx(i, n) == 1) {
            seq_t next = sum + (seq_t)i;
            if (next > (seq_t)n) break;
            sum = next;
        }
    }
    return sum;
}
