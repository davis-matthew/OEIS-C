/*
 * A004564  Expansion of sqrt(6) in base 5.
 * https://oeis.org/A004564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(6) * 5^(n-1)) mod 5, i.e. the n-th base-5
 *            digit of sqrt(6), computed by the digit-by-digit square-root
 *            method with an internal multiprecision remainder (r <- 25r +
 *            pair; d = max with (10p+d)d <= r; p <- 5p+d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   add_small, mul_add_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/add_small.h"
#include "../utils/mul_add_small.h"

static uint32_t *P = NULL, *R = NULL, *S = NULL;
static unsigned char *dig = NULL;
static size_t cap = 0;      /* limbs */
static idx_t ndig = 0;      /* digits computed so far */
static idx_t digcap = 0;

static int cmp_big(const uint32_t *A, const uint32_t *B, size_t len) {
    size_t i = len;
    while (i > 0) {
        i--;
        if (A[i] != B[i]) return A[i] > B[i] ? 1 : -1;
    }
    return 0;
}

static void sub_big(uint32_t *A, const uint32_t *B, size_t len) {
    uint64_t borrow = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        uint64_t t = (uint64_t)A[i] - B[i] - borrow;
        A[i] = (uint32_t)t;
        borrow = (t >> 63) & 1;
    }
}

static void ensure(idx_t n) {
    size_t need = (size_t)(n / 13) + 8;
    if (need > cap) {
        size_t newcap = need * 2;
        P = (uint32_t *)realloc(P, newcap * sizeof(uint32_t));
        R = (uint32_t *)realloc(R, newcap * sizeof(uint32_t));
        S = (uint32_t *)realloc(S, newcap * sizeof(uint32_t));
        memset(P + cap, 0, (newcap - cap) * sizeof(uint32_t));
        memset(R + cap, 0, (newcap - cap) * sizeof(uint32_t));
        memset(S + cap, 0, (newcap - cap) * sizeof(uint32_t));
        cap = newcap;
    }
    if (n + 1 > digcap) {
        idx_t nc = (n + 1) * 2;
        dig = (unsigned char *)realloc(dig, (size_t)nc);
        digcap = nc;
    }
}

seq_t a(idx_t n) {
    if (n > ndig) {
        ensure(n);
        while (ndig < n) {
            uint32_t pair = (ndig == 0) ? 6u : 0u;
            uint32_t d = 0;
            mul_add_small(R, 25u, pair, cap);
            memcpy(S, P, cap * sizeof(uint32_t));
            mul_add_small(S, 10u, 1u, cap);
            while (cmp_big(R, S, cap) >= 0) {
                sub_big(R, S, cap);
                add_small(S, 2u, cap);
                d++;
            }
            mul_add_small(P, 5u, d, cap);
            ndig++;
            dig[ndig] = (unsigned char)d;
        }
    }
    return (seq_t)dig[n];
}
