/*
 * A004713  Positions of ones in binary expansion of 1/sqrt(2).
 * https://oeis.org/A004713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   bit-by-bit search: a(n) = position of the n-th 1 bit of
 *            1/sqrt(2) = sqrt(2)/2, so bit k of 1/sqrt(2) is bit k of sqrt(2)
 *            = 1.0110101...; digit-by-digit binary square root of 2 with
 *            exact big integers
 * Range:     u8 n <= 132, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *R = 0, *Y = 0, *T = 0;
static size_t cap = 0;
static uint64_t *pos = 0;
static size_t pcount = 0, pcap = 0;
static uint64_t kbits = 0;

static void ensure_cap(size_t need) {
    if (need <= cap) return;
    size_t nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    R = (uint32_t *)realloc(R, nc * sizeof(uint32_t));
    Y = (uint32_t *)realloc(Y, nc * sizeof(uint32_t));
    T = (uint32_t *)realloc(T, nc * sizeof(uint32_t));
    memset(R + cap, 0, (nc - cap) * sizeof(uint32_t));
    memset(Y + cap, 0, (nc - cap) * sizeof(uint32_t));
    memset(T + cap, 0, (nc - cap) * sizeof(uint32_t));
    cap = nc;
}

static void add_pos(uint64_t p) {
    if (pcount == pcap) {
        pcap = pcap ? pcap * 2 : 64;
        pos = (uint64_t *)realloc(pos, pcap * sizeof(uint64_t));
    }
    pos[pcount++] = p;
}

static void step(void) {
    size_t len = (size_t)((kbits + 6) / 32 + 2);
    size_t i;
    ensure_cap(len + 1);
    /* R = 4R (next pair of radicand bits is 00) */
    for (i = len; i-- > 0;) {
        uint32_t lo = i ? (R[i - 1] >> 30) : 0;
        R[i] = (R[i] << 2) | lo;
    }
    /* T = R - (4Y + 1) */
    int64_t borrow = 0;
    for (i = 0; i < len; i++) {
        uint32_t y4 = (Y[i] << 2) | (i ? (Y[i - 1] >> 30) : 0);
        int64_t d = (int64_t)R[i] - (int64_t)y4 - (i == 0 ? 1 : 0) - borrow;
        if (d < 0) { d += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        T[i] = (uint32_t)d;
    }
    int bit = (borrow == 0);
    if (bit) memcpy(R, T, len * sizeof(uint32_t));
    /* Y = 2Y + bit */
    for (i = len; i-- > 0;) {
        uint32_t lo = i ? (Y[i - 1] >> 31) : 0;
        Y[i] = (Y[i] << 1) | lo;
    }
    Y[0] |= (uint32_t)bit;
    kbits++;
    if (bit) add_pos(kbits);
}

seq_t a(idx_t n) {
    if (kbits == 0) {
        ensure_cap(16);
        Y[0] = 1;   /* floor(sqrt(2)) = 1 */
        R[0] = 1;   /* 2 - 1^2 */
        kbits = 1;
        add_pos(1);
    }
    while ((idx_t)pcount < n) step();
    return (seq_t)pos[n - 1];
}
