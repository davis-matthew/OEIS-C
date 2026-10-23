/*
 * A118113  Even Fibbinary numbers + 1; also 2*Fibbinary(n) + 1.
 * https://oeis.org/A118113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A003714(n) + 1, where A003714(n) is the n‑th integer
 *            whose binary representation contains no consecutive 1 bits
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *fb = NULL;
    static size_t fb_len = 0;
    static size_t cap     = 0;
    static idx_t next_candidate = 0;

    if (cap == 0) {
        cap = 16;
        fb = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!fb) abort();
    }

    while (fb_len <= n) {
        while (1) {
            if ((next_candidate & (next_candidate >> 1)) == 0) {
                if (fb_len >= cap) {
                    cap <<= 1;
                    fb = (seq_t*)realloc(fb, cap * sizeof(seq_t));
                    if (!fb) abort();
                }
                fb[fb_len++] = (seq_t)next_candidate;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }

    seq_t base = fb[n];
    return (base << 1) + (seq_t)1;
}
