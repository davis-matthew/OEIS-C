/*
 * A004579  Expansion of sqrt(8) in base 4.
 * https://oeis.org/A004579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(8)*4^(n-1)) mod 4 = isqrt(2^(4n-1)) mod 4;
 *            computed by binary digit-by-digit square root with an internal
 *            bignum
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_words, shl_into, sub_words
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/cmp_words.h"
#include "../utils/shl_into.h"
#include "../utils/sub_words.h"

static uint32_t *Rw = NULL, *Mw = NULL, *Tw = NULL;
static size_t capw = 0;
static unsigned char *bits = NULL;
static size_t bitcap = 0;
static size_t nsteps = 0;

static void ensure_words(size_t w) {
    if (w <= capw) return;
    size_t nc = capw ? capw : 16;
    while (nc < w) nc *= 2;
    Rw = (uint32_t *)realloc(Rw, nc * sizeof(uint32_t));
    Mw = (uint32_t *)realloc(Mw, nc * sizeof(uint32_t));
    Tw = (uint32_t *)realloc(Tw, nc * sizeof(uint32_t));
    memset(Rw + capw, 0, (nc - capw) * sizeof(uint32_t));
    memset(Mw + capw, 0, (nc - capw) * sizeof(uint32_t));
    memset(Tw + capw, 0, (nc - capw) * sizeof(uint32_t));
    capw = nc;
}

static void do_step(void) {
    size_t s = nsteps + 1;
    size_t w = s / 32 + 3;
    ensure_words(w);
    if (s > bitcap) {
        size_t nb = bitcap ? bitcap * 2 : 256;
        while (nb < s) nb *= 2;
        bits = (unsigned char *)realloc(bits, nb);
        bitcap = nb;
    }
    uint32_t pair = (s == 1) ? 2u : 0u;
    shl_into(Mw, Mw, w, 2);
    Mw[0] |= pair;
    shl_into(Tw, Rw, w, 2);
    Tw[0] |= 1u;
    unsigned char b = 0;
    if (cmp_words(Mw, Tw, w) >= 0) {
        sub_words(Mw, Tw, w);
        b = 1;
    }
    shl_into(Rw, Rw, w, 1);
    Rw[0] |= b;
    bits[s - 1] = b;
    nsteps = s;
}

seq_t a(idx_t n) {
    size_t need = (size_t)(2 * n);
    while (nsteps < need) do_step();
    unsigned d = 2u * bits[2 * n - 2] + bits[2 * n - 1];
    return (seq_t)d;
}
