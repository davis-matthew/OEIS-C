/*
 * A033847  Numbers whose prime factors are 2 and 7.
 * https://oeis.org/A033847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14 * h(n-1) where h(k) enumerates 2^i*7^j (i,j ≥ 0) in *
 *            ascending order, starting from 1 (h(0)=1)
 * Range:     u8 n <= 7, u16 n <= 35, u32 n <= 161, u64 n <= 687,
 *            u128 n <= 2832, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *h = NULL;
    static size_t h_len = 0;
    static size_t h_cap = 0;
    static size_t i2 = 0, i7 = 0;
    const seq_t TWO   = (seq_t)2;
    const seq_t SEVEN = (seq_t)7;
    const seq_t F14   = (seq_t)14;

    if (h_len == 0) {
        h_cap = 16;
        h = (seq_t *)malloc(h_cap * sizeof(seq_t));
        h[0] = (seq_t)1;            /* h(0) = 1 */
        h_len = 1;
    }

    while (h_len < (size_t)n) {
        seq_t next2 = h[i2] * TWO;
        seq_t next7 = h[i7] * SEVEN;
        seq_t next  = (next2 < next7) ? next2 : next7;

        if (h_len == h_cap) {
            h_cap *= 2;
            h = (seq_t *)realloc(h, h_cap * sizeof(seq_t));
        }
        h[h_len++] = next;

        if (next == next2) i2++;
        if (next == next7) i7++;
    }

    return h[(size_t)(n - 1)] * F14;
}
