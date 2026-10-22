/*
 * A004539  Expansion of sqrt(2) in base 2.
 * https://oeis.org/A004539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(2)*2^(n-1)) mod 2 = A000196(2^(2n-1)) mod 2,
 *            computed by exact binary digit-by-digit integer square root
 *            (memoized)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_words
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/grow_words.h"

static uint32_t *Ybuf = NULL, *Rbuf = NULL, *Tbuf = NULL;
static size_t L = 0;
static unsigned char *bitsbuf = NULL;
static size_t cnt = 0, bcap = 0;

static void ensure_bits(size_t n) {
    if (n > bcap) {
        size_t nc = bcap ? bcap : 64;
        while (nc < n) nc *= 2;
        bitsbuf = (unsigned char *)realloc(bitsbuf, nc);
        bcap = nc;
    }
    while (cnt < n) {
        size_t need = (cnt + 1) / 32 + 3;
        if (need > L) {
            size_t nl = need * 2;
            Ybuf = grow_words(Ybuf, L, nl);
            Rbuf = grow_words(Rbuf, L, nl);
            Tbuf = grow_words(Tbuf, L, nl);
            L = nl;
        }
        uint32_t pair = (cnt == 0) ? 2u : 0u;
        size_t i;
        /* R = 4R + pair */
        uint32_t carry = 0;
        for (i = 0; i < L; i++) {
            uint32_t v = Rbuf[i];
            Rbuf[i] = (v << 2) | carry;
            carry = v >> 30;
        }
        Rbuf[0] |= pair;
        /* T = 4Y + 1 */
        carry = 0;
        for (i = 0; i < L; i++) {
            uint32_t v = Ybuf[i];
            Tbuf[i] = (v << 2) | carry;
            carry = v >> 30;
        }
        Tbuf[0] |= 1u;
        /* compare R >= T */
        int ge = 1;
        for (i = L; i-- > 0;) {
            if (Rbuf[i] != Tbuf[i]) { ge = Rbuf[i] > Tbuf[i]; break; }
        }
        unsigned char bit = 0;
        if (ge) {
            uint64_t borrow = 0;
            for (i = 0; i < L; i++) {
                uint64_t d = (uint64_t)Rbuf[i] - (uint64_t)Tbuf[i] - borrow;
                Rbuf[i] = (uint32_t)d;
                borrow = (d >> 63) & 1u;
            }
            bit = 1;
        }
        /* Y = 2Y + bit */
        carry = 0;
        for (i = 0; i < L; i++) {
            uint32_t v = Ybuf[i];
            Ybuf[i] = (v << 1) | carry;
            carry = v >> 31;
        }
        Ybuf[0] |= bit;
        bitsbuf[cnt++] = bit;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    ensure_bits((size_t)n);
    return (seq_t)bitsbuf[n - 1];
}
