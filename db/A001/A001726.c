/*
 * A001726  Weight distribution of [ 64,22,16 ] 2nd-order Reed-Muller code of
 *          length 64.
 * https://oeis.org/A001726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of codewords of weight 4n in the [64,22,16]
 *            second-order Reed-Muller code RM(2,6); computed by enumerating
 *            all 2^22 codewords (Gray-code walk over the 22 generator rows 1,
 *            x_i, x_i*x_j) and counting weights
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t a001726_cnt[65];
static int a001726_done = 0;

static void a001726_build(void) {
    uint64_t gen[22];
    int g = 0, i, j, p;
    uint64_t xv[6];
    for (i = 0; i < 6; i++) {
        xv[i] = 0;
        for (p = 0; p < 64; p++)
            if ((p >> i) & 1) xv[i] |= (uint64_t)1 << p;
    }
    gen[g++] = ~(uint64_t)0;
    for (i = 0; i < 6; i++) gen[g++] = xv[i];
    for (i = 0; i < 6; i++)
        for (j = i + 1; j < 6; j++) gen[g++] = xv[i] & xv[j];

    for (i = 0; i <= 64; i++) a001726_cnt[i] = 0;
    {
        uint64_t w = 0;
        uint32_t k, total = (uint32_t)1 << 22;
        a001726_cnt[0]++;
        for (k = 1; k < total; k++) {
            w ^= gen[__builtin_ctz(k)];
            a001726_cnt[__builtin_popcountll(w)]++;
        }
    }
    a001726_done = 1;
}

seq_t a(idx_t n) {
    if (!a001726_done) a001726_build();
    if (n > 16) return (seq_t)0;
    return (seq_t)a001726_cnt[(int)(4 * n)];
}
