/*
 * A002101  Nearest integer to 4 * Pi * n^3 / 3.
 * https://oeis.org/A002101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(4*Pi*n^3/3) = 4*n^3 + round(f*n^3), f = 4*(Pi-3)/3,
 *            f taken to 480 bits
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t a002101_pi_frac[16] = {
    0x243F6A88u, 0x85A308D3u, 0x13198A2Eu, 0x03707344u,
    0xA4093822u, 0x299F31D0u, 0x082EFA98u, 0xEC4E6C89u,
    0x452821E6u, 0x38D01377u, 0xBE5466CFu, 0x34E90C6Cu,
    0xC0AC29B7u, 0xC97C50DDu, 0x3F84D5B5u, 0xB5470917u
};

static uint32_t a002101_f[16];
static int a002101_ready = 0;

static void a002101_init(void) {
    uint32_t t[16];
    uint32_t carry = 0;
    uint64_t rem = 0;
    int i;
    /* t = 4 * frac(Pi), still < 1 since frac(Pi) < 1/4 */
    for (i = 15; i >= 0; i--) {
        uint32_t w = a002101_pi_frac[i];
        t[i] = (w << 2) | carry;
        carry = w >> 30;
    }
    /* f = t / 3 */
    for (i = 0; i < 16; i++) {
        uint64_t cur = (rem << 32) | t[i];
        a002101_f[i] = (uint32_t)(cur / 3);
        rem = cur % 3;
    }
    a002101_ready = 1;
}

static int a002101_bit(int i) { /* i >= 1, weight 2^-i */
    int k = i - 1;
    return (int)((a002101_f[k / 32] >> (31 - (k % 32))) & 1u);
}

seq_t a(idx_t n) {
    seq_t N, q;
    int i;
    const int K = 480;
    if (!a002101_ready) a002101_init();
    N = (seq_t)n * (seq_t)n * (seq_t)n;
    q = 0;
    /* q = floor(sum_{i>=2} b_i * N * 2^-(i-1)) */
    for (i = K; i >= 2; i--) {
        if (a002101_bit(i)) q += N;
        if (i > 2) q /= 2;
    }
    /* now q holds floor(sum_{i>=2} b_i N 2^-(i-1)) after last halving at i=3..K; adjust */
    q /= 2;
    if (a002101_bit(1)) q += N;
    /* q = floor(2*f*N); rounded f*N = floor((q+1)/2) */
    q = (q + 1) / 2;
    return N * 4 + q;
}
