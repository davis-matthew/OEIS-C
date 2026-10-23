/*
 * A161765  a(n) is the smallest multiple of {the number of 1's in the binary
 *          representation of n} that is >= n.
 * https://oeis.org/A161765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A000120(n)*ceil(n/A000120(n))
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest multiple of the popcount of n that is >= n. */

#include <stdint.h>   /* for standard integer types, if needed */

static inline int popcount_idx(idx_t x) {
    int cnt = 0;
    while (x) {
        cnt++;
        x &= x - 1;  /* drop the lowest set bit */
    }
    return cnt;
}

seq_t a(idx_t n) {
    /* For n >= 1, compute k = popcount(n), then ceil(n/k)*k. */
    if (n == 0) return 0;  /* safety, though offset starts at 1 */
    int pc = popcount_idx(n);
    idx_t k = (idx_t)pc;
    idx_t t = (n + k - 1) / k;   /* ceil division */
    return (seq_t)t * (seq_t)k;
}
