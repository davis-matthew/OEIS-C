/*
 * A217921  Number of steps to calculate A175872(n).
 * https://oeis.org/A217921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps to reach an all‑1 vector by iterated
 *            run‑length encoding of the binary digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static inline idx_t msb_idx(idx_t x) {
    /* returns floor(log2(x)) for x > 0; caller guarantees x != 0 */
    idx_t r = 0;
    while (x > 1) {
        x >>= 1;
        ++r;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* Extract the binary digits of n (most significant first). */
    seq_t bits[64];
    size_t len = 0;
    idx_t msb = msb_idx(n);
    for (idx_t i = msb; ; --i) {
        bits[len++] = (seq_t)((n >> i) & 1ULL);
        if (i == 0) break;
    }

    seq_t steps = 0;
    while (1) {
        /* Stop when every entry equals 1. */
        bool all_one = true;
        for (size_t i = 0; i < len; ++i) {
            if (bits[i] != (seq_t)1) {
                all_one = false;
                break;
            }
        }
        if (all_one) break;

        /* Run‑length encode the current vector. */
        seq_t new_bits[64];
        size_t new_len = 0;
        size_t i = 0;
        while (i < len) {
            seq_t val = bits[i];
            size_t j = i + 1;
            while (j < len && bits[j] == val) ++j;
            new_bits[new_len++] = (seq_t)(j - i);
            i = j;
        }

        /* Prepare for next step. */
        for (size_t k = 0; k < new_len; ++k) bits[k] = new_bits[k];
        len = new_len;
        ++steps;
    }

    return steps;
}
