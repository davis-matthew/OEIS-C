/*
 * A004541  Expansion of sqrt(2) in base 4.
 * https://oeis.org/A004541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(1) = floor(sqrt(2)) = 1; for n >= 2, a(n) =
 *            floor(4^(n-1)*sqrt(2)) mod 4, computed with an exact binary
 *            digit-by-digit integer square root of 2*4^k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *sq_rem = NULL, *sq_root = NULL;
static size_t sq_cap = 0;
static unsigned char *sq_bits = NULL;
static size_t sq_nbits = 0, sq_bcap = 0;

static void sq_ensure_limbs(size_t need) {
    if (need <= sq_cap) return;
    size_t nc = sq_cap ? sq_cap : 16;
    while (nc < need) nc *= 2;
    sq_rem = (uint32_t *)realloc(sq_rem, nc * sizeof(uint32_t));
    sq_root = (uint32_t *)realloc(sq_root, nc * sizeof(uint32_t));
    memset(sq_rem + sq_cap, 0, (nc - sq_cap) * sizeof(uint32_t));
    memset(sq_root + sq_cap, 0, (nc - sq_cap) * sizeof(uint32_t));
    sq_cap = nc;
}

/* One step of binary square root: bring down a pair of bits, emit one root bit.
   sq_bits[0] is the integer bit of sqrt(2); sq_bits[i] is the i-th fractional bit. */
static void sq_step(uint32_t pair) {
    size_t L = sq_nbits / 32 + 3;
    size_t j;
    int ge;
    sq_ensure_limbs(L);
    if (sq_nbits >= sq_bcap) {
        size_t nb = sq_bcap ? sq_bcap * 2 : 256;
        sq_bits = (unsigned char *)realloc(sq_bits, nb);
        sq_bcap = nb;
    }
    /* rem = rem*4 + pair */
    for (j = L; j-- > 0;) {
        sq_rem[j] = (sq_rem[j] << 2) | (j ? (sq_rem[j - 1] >> 30) : 0);
    }
    sq_rem[0] |= pair;
    /* compare rem with trial = root*4 + 1 */
    ge = 1;
    for (j = L; j-- > 0;) {
        uint32_t t = (sq_root[j] << 2) | (j ? (sq_root[j - 1] >> 30) : 1u);
        if (sq_rem[j] != t) { ge = sq_rem[j] > t; break; }
    }
    if (ge) {
        uint64_t borrow = 0;
        for (j = 0; j < L; j++) {
            uint32_t t = (sq_root[j] << 2) | (j ? (sq_root[j - 1] >> 30) : 1u);
            uint64_t d = (uint64_t)sq_rem[j] - t - borrow;
            sq_rem[j] = (uint32_t)d;
            borrow = (d >> 63) & 1;
        }
    }
    for (j = L; j-- > 0;) {
        sq_root[j] = (sq_root[j] << 1) | (j ? (sq_root[j - 1] >> 31) : 0);
    }
    sq_root[0] |= (uint32_t)ge;
    sq_bits[sq_nbits++] = (unsigned char)ge;
}

seq_t a(idx_t n) {
    size_t need = (size_t)(2 * n - 1);
    while (sq_nbits < need) {
        sq_step(sq_nbits == 0 ? 2u : 0u);
    }
    if (n == 1) return (seq_t)sq_bits[0];
    {
        int d = 2 * sq_bits[2 * n - 3] + sq_bits[2 * n - 2];
        return (seq_t)d;
    }
}
