/*
 * A073334  The so-called "rhythmic infinity system" of Danish composer Per
 *          Nørgård [Noergaard].
 * https://oeis.org/A073334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hear
 * Formula:   a(n) = F(c(n)+4) where F(k) is the k-th Fibonacci number (F1=1,
 *            F2=1) and c(n) counts the blocks of consecutive identical bits
 *            in the binary representation of n (c(0)=0)
 * Range:     u8 n <= 681, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Count blocks of identical bits in n's binary expansion. */
    uint64_t nn = n;
    uint64_t blocks;
    if (nn == 0) {
        blocks = 0;
    } else {
        /* Find position of most significant bit. */
        unsigned msb = 63 - __builtin_clzll(nn);
        uint64_t prev_bit = (nn >> msb) & 1ULL;
        blocks = 1;
        for (int i = msb - 1; i >= 0; --i) {
            uint64_t bit = (nn >> i) & 1ULL;
            if (bit != prev_bit) {
                ++blocks;
                prev_bit = bit;
            }
        }
    }

    /* Compute Fibonacci number F(blocks + 4). */
    uint64_t k = blocks + 4ULL;
    seq_t f_prev = 1;   /* F1 */
    seq_t f_curr = 1;   /* F2 */
    if (k <= 2) {
        return 1;
    }
    seq_t f_next;
    for (uint64_t i = 3; i <= k; ++i) {
        f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
    return f_curr;
}
