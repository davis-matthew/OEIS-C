/*
 * A075488  Length of iteration list when Collatz-function is iterated with
 *          initial value -1+3^n.
 * https://oeis.org/A075488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008908(3^n-1): count the terms of the Collatz trajectory
 *            of 3^n-1, using an internal multi-precision integer so the start
 *            value and the values along the way never overflow
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 992 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void A075488_mul3add(uint32_t **px, size_t *len, size_t *cap, uint32_t add)
{
    uint32_t *x = *px;
    uint64_t carry = add;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint64_t t = (uint64_t)x[i] * 3u + carry;
        x[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) {
        if (*len >= *cap) {
            size_t nc = *cap * 2 + 8;
            x = (uint32_t *)realloc(x, nc * sizeof(uint32_t));
            *px = x;
            *cap = nc;
        }
        x[(*len)++] = (uint32_t)carry;
    }
}

seq_t a(idx_t n)
{
    size_t cap = (size_t)(n * 16 / 10 / 32) + 16;
    uint32_t *x = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t len = 1;
    idx_t i;
    uint64_t count = 1;

    x[0] = 1;
    for (i = 0; i < n; i++)
        A075488_mul3add(&x, &len, &cap, 0);
    x[0] -= 1; /* 3^n is odd, so no borrow */

    for (;;) {
        if (len == 1 && x[0] == 1)
            break;
        if (x[0] & 1u) {
            A075488_mul3add(&x, &len, &cap, 1);
            count++;
        } else {
            size_t w = 0;
            unsigned b = 0;
            size_t j, newlen;
            while (x[w] == 0) w++;
            {
                uint32_t v = x[w];
                while ((v & 1u) == 0) { v >>= 1; b++; }
            }
            count += (uint64_t)w * 32u + b;
            newlen = len - w;
            for (j = 0; j < newlen; j++) {
                uint32_t lo = x[j + w];
                uint32_t hi = (j + w + 1 < len) ? x[j + w + 1] : 0;
                x[j] = b ? ((lo >> b) | (hi << (32 - b))) : lo;
            }
            len = newlen;
            while (len > 1 && x[len - 1] == 0) len--;
        }
    }
    free(x);
    return (seq_t)count;
}
