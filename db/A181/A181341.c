/*
 * A181341  Divide the left and right binary concatenations of 1 to n into
 *          sections alternating between being composed entirely of
 *          consecutive digits identical in both concatenations and
 *          consecutive digits different in both concatenations, then count
 *          the sections in one of the concatenations.
 * https://oeis.org/A181341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of alternations between equal and different bits
 *            in the binary concatenation of 1..n from left to right and from
 *            right to left. The value returned is the number of runs in that
 *            comparison
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   bits_in
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/bits_in.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n=0, but safe */

    /* total number of bits in the full concatenation */
    size_t total_bits = 0;
    for (idx_t i = 1; i <= n; ++i) total_bits += bits_in(i);

    uint8_t *l = (uint8_t *)malloc(total_bits);
    uint8_t *r = (uint8_t *)malloc(total_bits);
    if (!l || !r) { free(l); free(r); return 0; }

    /* build left concatenation (1..n) */
    size_t pos = 0;
    for (idx_t i = 1; i <= n; ++i) {
        size_t bits = bits_in(i);
        idx_t y = i;
        for (size_t k = bits; k > 0; --k)
            l[pos++] = (uint8_t)((y >> (k - 1)) & 1);
    }

    /* build right concatenation (n..1) */
    pos = 0;
    for (idx_t i = n; i > 0; --i) {
        size_t bits = bits_in(i);
        idx_t y = i;
        for (size_t k = bits; k > 0; --k)
            r[pos++] = (uint8_t)((y >> (k - 1)) & 1);
    }

    /* count runs of equal / different bits */
    seq_t runs = 1;                     /* at least one run */
    int prev_eq = (l[0] == r[0]) ? 1 : 0;
    for (size_t i = 1; i < total_bits; ++i) {
        int cur_eq = (l[i] == r[i]) ? 1 : 0;
        if (cur_eq != prev_eq) {
            ++runs;
            prev_eq = cur_eq;
        }
    }

    free(l);
    free(r);
    return runs;
}
