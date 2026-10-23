/*
 * A044858  Positive integers having the same number of base-2 runs of odd
 *          length as even.
 * https://oeis.org/A044858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044858: nth positive integer whose binary representation
   contains an equal number of runs of odd length and runs of even length.
   Computed by brute‑force enumeration. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool has_equal_runs(uint64_t x)
{
    if (x == 0) return false;
    int msb = 63 - __builtin_clzll(x);
    int cur_bit = (x >> msb) & 1;
    int cur_len = 1;
    int odd = 0, even = 0;

    for (int i = msb - 1; i >= 0; --i) {
        int bit = (x >> i) & 1;
        if (bit != cur_bit) {
            if ((cur_len & 1) == 0) even++; else odd++;
            cur_bit = bit;
            cur_len = 1;
        } else {
            cur_len++;
        }
    }
    if ((cur_len & 1) == 0) even++; else odd++;

    return odd == even;
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_cnt = 0;
    static uint64_t candidate = 1;

    if (n == 0) return (seq_t)0;  /* undefined, but harmless */

    while (seq_cnt < n) {
        if (has_equal_runs(candidate)) {
            if (seq_cnt == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                seq_cap = new_cap;
            }
            seq[seq_cnt++] = (seq_t)candidate;
        }
        ++candidate;
    }
    return seq[n - 1];
}
