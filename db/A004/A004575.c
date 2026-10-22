/*
 * A004575  Expansion of sqrt(7) in base 8.
 * https://oeis.org/A004575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(1) = 2; for n >= 2, a(n) = floor(sqrt(7)*8^(n-1)) mod 8,
 *            computed by binary digit-by-digit integer square root: r_0 = 2,
 *            m_0 = 3; if m_k > r_k then r_{k+1} = 2r_k+1, m_{k+1} =
 *            4(m_k-r_k)-1 else r_{k+1} = 2r_k, m_{k+1} = 4m_k; r_k =
 *            floor(sqrt(7)*2^k)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *R = NULL, *M = NULL;
static size_t cap = 0;
static unsigned char *bits = NULL;
static size_t nbits = 0, bitcap = 0;

static void ensure_cap(size_t need_bits)
{
    size_t need = need_bits / 32 + 2;
    if (need <= cap) return;
    size_t nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    R = (uint32_t *)realloc(R, nc * sizeof(uint32_t));
    M = (uint32_t *)realloc(M, nc * sizeof(uint32_t));
    memset(R + cap, 0, (nc - cap) * sizeof(uint32_t));
    memset(M + cap, 0, (nc - cap) * sizeof(uint32_t));
    cap = nc;
}

static int cmp_big(const uint32_t *x, const uint32_t *y)
{
    size_t i = cap;
    while (i-- > 0) {
        if (x[i] != y[i]) return x[i] > y[i] ? 1 : -1;
    }
    return 0;
}

static void shl_big(uint32_t *x, int s, uint32_t in)
{
    uint32_t carry = in;
    for (size_t i = 0; i < cap; i++) {
        uint32_t v = x[i];
        x[i] = (v << s) | carry;
        carry = v >> (32 - s);
    }
}

static void step(void)
{
    ensure_cap(nbits + 16);
    if (cmp_big(M, R) > 0) {
        uint64_t borrow = 0;
        for (size_t i = 0; i < cap; i++) {
            uint64_t d = (uint64_t)M[i] - R[i] - borrow;
            M[i] = (uint32_t)d;
            borrow = (d >> 63) & 1;
        }
        shl_big(M, 2, 0);
        for (size_t i = 0; i < cap; i++) {
            if (M[i]-- != 0) break;
        }
        shl_big(R, 1, 1);
        bits[nbits++] = 1;
    } else {
        shl_big(M, 2, 0);
        shl_big(R, 1, 0);
        bits[nbits++] = 0;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)2;
    if (cap == 0) {
        ensure_cap(64);
        R[0] = 2;
        M[0] = 3;
    }
    size_t need = (size_t)(3 * (n - 1));
    if (need > bitcap) {
        size_t nb = bitcap ? bitcap : 256;
        while (nb < need) nb *= 2;
        bits = (unsigned char *)realloc(bits, nb);
        bitcap = nb;
    }
    while (nbits < need) step();
    size_t b = need - 3;
    int d = bits[b] * 4 + bits[b + 1] * 2 + bits[b + 2];
    return (seq_t)d;
}
