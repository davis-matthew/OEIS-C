/*
 * A003356  Numbers that are the sum of 11 positive 5th powers.
 * https://oeis.org/A003356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve. A bitmask DP over v <= L marks which k (0..11) allow v to
 *            be written as a sum of exactly k positive 5th powers. The terms
 *            are the v whose mask has bit 11 set, in increasing order. L
 *            doubles until there are at least n terms
 * Range:     u8 n <= 9, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *A003356_vals = NULL;
static size_t A003356_cnt = 0;
static uint64_t A003356_lim = 0;

static void A003356_build(uint64_t L) {
    uint16_t *m = (uint16_t *)calloc((size_t)L + 1, sizeof(uint16_t));
    uint64_t b, v, p;
    size_t c = 0;
    m[0] = 1;
    for (b = 1; ; b++) {
        p = b * b * b * b * b;
        if (p > L) break;
        for (v = p; v <= L; v++)
            m[v] |= (uint16_t)((m[v - p] << 1) & 0xFFF);
    }
    for (v = 0; v <= L; v++)
        if (m[v] & 0x800) c++;
    free(A003356_vals);
    A003356_vals = (uint64_t *)malloc((c ? c : 1) * sizeof(uint64_t));
    c = 0;
    for (v = 0; v <= L; v++)
        if (m[v] & 0x800) A003356_vals[c++] = v;
    A003356_cnt = c;
    A003356_lim = L;
    free(m);
}

seq_t a(idx_t n) {
    while ((idx_t)A003356_cnt < n) {
        uint64_t L = A003356_lim ? A003356_lim * 2 : (uint64_t)65536;
        A003356_build(L);
    }
    return (seq_t)A003356_vals[n - 1];
}
