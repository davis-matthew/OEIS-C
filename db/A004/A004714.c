/*
 * A004714  Positions of ones in binary expansion of the reciprocal of the
 *          golden ratio (0.618...).
 * https://oeis.org/A004714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over binary digits: a(n) = position of the n-th 1-bit of
 *            1/phi = (sqrt(5)-1)/2 = 0.1001111000110111...; a(1) = 1 and, for
 *            k >= 2, bit k of 1/phi equals fractional bit k-1 of sqrt(5),
 *            generated exactly by the digit-by-digit binary square root with
 *            big integers
 * Range:     u8 n <= 124, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *SY = 0, *SR = 0, *ST = 0;
static size_t s_len = 0, s_cap = 0;
static uint64_t s_steps = 0;
static uint64_t *s_pos = 0;
static uint64_t s_pcnt = 0, s_pcap = 0;

static void s_ensure(size_t need) {
    if (need <= s_cap) return;
    size_t nc = need * 2 + 16;
    SY = (uint32_t *)realloc(SY, nc * sizeof(uint32_t));
    SR = (uint32_t *)realloc(SR, nc * sizeof(uint32_t));
    ST = (uint32_t *)realloc(ST, nc * sizeof(uint32_t));
    memset(SY + s_cap, 0, (nc - s_cap) * sizeof(uint32_t));
    memset(SR + s_cap, 0, (nc - s_cap) * sizeof(uint32_t));
    memset(ST + s_cap, 0, (nc - s_cap) * sizeof(uint32_t));
    s_cap = nc;
}

/* one step of the binary square root of 5 (radicand pairs 01 01 00 00 ...) */
static int s_step(void) {
    size_t i;
    if (s_len == 0 || (SY[s_len - 1] | SR[s_len - 1]) != 0) {
        s_ensure(s_len + 1);
        SY[s_len] = SR[s_len] = ST[s_len] = 0;
        s_len++;
    }
    uint32_t cr = (s_steps < 2) ? 1u : 0u, ct = 1u;
    s_steps++;
    for (i = 0; i < s_len; i++) {
        uint32_t v = SR[i];
        SR[i] = (v << 2) | cr;
        cr = v >> 30;
        v = SY[i];
        ST[i] = (v << 2) | ct;
        ct = v >> 30;
    }
    int ge = 1;
    for (i = s_len; i-- > 0;) {
        if (SR[i] != ST[i]) { ge = SR[i] > ST[i]; break; }
    }
    if (ge) {
        uint64_t borrow = 0;
        for (i = 0; i < s_len; i++) {
            uint64_t d = (uint64_t)SR[i] - ST[i] - borrow;
            SR[i] = (uint32_t)d;
            borrow = (d >> 32) & 1u;
        }
    }
    uint32_t c = (uint32_t)ge;
    for (i = 0; i < s_len; i++) {
        uint32_t v = SY[i];
        SY[i] = (v << 1) | c;
        c = v >> 31;
    }
    return ge;
}

static void s_push(uint64_t p) {
    if (s_pcnt == s_pcap) {
        s_pcap = s_pcap * 2 + 64;
        s_pos = (uint64_t *)realloc(s_pos, s_pcap * sizeof(uint64_t));
    }
    s_pos[s_pcnt++] = p;
}

seq_t a(idx_t n) {
    if (s_pcnt == 0) s_push(1);
    while (s_pcnt < n) {
        int b = s_step();
        if (s_steps >= 3 && b) s_push(s_steps - 1);
    }
    return (seq_t)s_pos[n - 1];
}
