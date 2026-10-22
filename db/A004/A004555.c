/*
 * A004555  Expansion of sqrt(5) in base 2.
 * https://oeis.org/A004555
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(5) * 2^(n-3)) mod 2, i.e. the binary digits of
 *            sqrt(5), computed with the exact digit-by-digit (bit-pair)
 *            square-root algorithm on multiword integers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *P = NULL, *R = NULL, *T = NULL;
static size_t cap = 0;              /* words in each of P, R, T */
static unsigned char *bits = NULL;  /* bits[j] = a(j+2) */
static size_t nbits = 0, bcap = 0;

static void grow_words(size_t newcap) {
    P = (uint32_t *)realloc(P, newcap * sizeof(uint32_t));
    R = (uint32_t *)realloc(R, newcap * sizeof(uint32_t));
    T = (uint32_t *)realloc(T, newcap * sizeof(uint32_t));
    memset(P + cap, 0, (newcap - cap) * sizeof(uint32_t));
    memset(R + cap, 0, (newcap - cap) * sizeof(uint32_t));
    memset(T + cap, 0, (newcap - cap) * sizeof(uint32_t));
    cap = newcap;
}

static void shl_add(uint32_t *x, unsigned s, uint32_t add) {
    uint32_t carry = add;
    size_t i;
    for (i = 0; i < cap; i++) {
        uint32_t w = x[i];
        x[i] = (w << s) | carry;
        carry = w >> (32 - s);
    }
}

static void step(void) {
    /* next base-4 digit of 5 = (11)_4 followed by zeros */
    uint32_t d = (nbits < 2) ? 1u : 0u;
    unsigned char b;
    size_t i;
    if ((nbits + 8) >= cap * 32) grow_words(cap ? cap * 2 : 4);
    shl_add(R, 2, d);
    for (i = 0; i < cap; i++) T[i] = P[i];
    shl_add(T, 2, 1u);
    {
        int ge = 1;
        i = cap;
        while (i > 0) {
            i--;
            if (R[i] != T[i]) { ge = R[i] > T[i]; break; }
        }
        if (ge) {
            uint64_t borrow = 0;
            for (i = 0; i < cap; i++) {
                uint64_t diff = (uint64_t)R[i] - T[i] - borrow;
                R[i] = (uint32_t)diff;
                borrow = (diff >> 63) & 1;
            }
            shl_add(P, 1, 1u);
            b = 1;
        } else {
            shl_add(P, 1, 0u);
            b = 0;
        }
    }
    if (nbits >= bcap) {
        bcap = bcap ? bcap * 2 : 256;
        bits = (unsigned char *)realloc(bits, bcap);
    }
    bits[nbits++] = b;
}

seq_t a(idx_t n) {
    size_t j;
    if (n < 2) return (seq_t)0;
    j = (size_t)(n - 2);
    while (nbits <= j) step();
    return (seq_t)bits[j];
}
