/*
 * A006539  Numerators of worst case for Engel expansion.
 * https://oeis.org/A006539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   search. For each denominator q = 1, 2, 3, ... compute the Engel
 *            expansion length L(p/q) for every p < q, using L(p/q) = 1 +
 *            L(((-q) mod p)/q) with L(0/q) = 0, since u = ceil(q/p) and p*u -
 *            q = (-q) mod p. a(n) is the numerator p of the first fraction
 *            p/q, smallest q first and then smallest p, who...
 * Range:     u8 n <= 19, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *firstp = NULL;   /* firstp[len] = numerator, 0 if not found yet */
static size_t fcap = 0;
static uint64_t qdone = 0;
static uint32_t *Lbuf = NULL;
static size_t Lcap = 0;

static void ensure_f(size_t len) {
    if (len < fcap) return;
    size_t nc = fcap ? fcap : 64;
    while (nc <= len) nc *= 2;
    firstp = (uint64_t *)realloc(firstp, nc * sizeof(uint64_t));
    for (size_t i = fcap; i < nc; i++) firstp[i] = 0;
    fcap = nc;
}

static void scan_next_q(void) {
    uint64_t q = ++qdone;
    if ((size_t)q + 1 > Lcap) {
        size_t nc = Lcap ? Lcap : 64;
        while (nc < (size_t)q + 1) nc *= 2;
        Lbuf = (uint32_t *)realloc(Lbuf, nc * sizeof(uint32_t));
        Lcap = nc;
    }
    Lbuf[0] = 0;
    uint64_t pmax = (q == 1) ? 1 : q - 1;
    for (uint64_t p = 1; p <= pmax; p++) {
        uint64_t r = q % p;
        uint64_t np = r ? p - r : 0;
        uint32_t len = 1 + Lbuf[np];
        Lbuf[p] = len;
        ensure_f(len);
        if (firstp[len] == 0) firstp[len] = p;
    }
}

seq_t a(idx_t n) {
    size_t need = (size_t)n + 1;
    ensure_f(need);
    while (firstp[need] == 0) {
        scan_next_q();
        ensure_f(need);
    }
    return (seq_t)firstp[need];
}
