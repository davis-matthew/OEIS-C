/*
 * A003067  Problimes (second definition).
 * https://oeis.org/A003067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(1) = 2; a(n) = round(a(n-1) + 1/Product_{k=1..n-1} (1 -
 *            1/a(k))), rounding to nearest with ties to even. The product is
 *            kept as an exact fraction N/D = Product a(k)/(a(k)-1) using
 *            simple bignums
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   A3067_cmp, A3067_mul, A3067_sub, A3067_win
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/A3067_cmp.h"
#include "../utils/A3067_mul.h"
#include "../utils/A3067_sub.h"
#include "../utils/A3067_win.h"

static uint32_t *A3067_N, *A3067_D, *A3067_R, *A3067_T;
static size_t A3067_ln, A3067_ld, A3067_cap;
static uint64_t *A3067_terms;
static idx_t A3067_cnt, A3067_tcap;

static void A3067_ensure(size_t need) {
    if (need <= A3067_cap) return;
    size_t nc = A3067_cap ? A3067_cap : 16;
    while (nc < need) nc *= 2;
    A3067_N = (uint32_t *)realloc(A3067_N, nc * sizeof(uint32_t));
    A3067_D = (uint32_t *)realloc(A3067_D, nc * sizeof(uint32_t));
    A3067_R = (uint32_t *)realloc(A3067_R, nc * sizeof(uint32_t));
    A3067_T = (uint32_t *)realloc(A3067_T, nc * sizeof(uint32_t));
    A3067_cap = nc;
}

static void A3067_extend(void) {
    uint64_t prev = A3067_terms[A3067_cnt - 1];
    size_t ln = A3067_ln, ld = A3067_ld;
    A3067_ensure(ln + 4);
    uint32_t top = A3067_N[ln - 1];
    size_t bl = (ln - 1) * 32;
    while (top) { bl++; top >>= 1; }
    size_t s = bl > 63 ? bl - 63 : 0;
    uint64_t nt = A3067_win(A3067_N, ln, s), dt = A3067_win(A3067_D, ld, s);
    uint64_t q = nt / (dt + 1);
    /* T = q * D */
    memcpy(A3067_T, A3067_D, ld * sizeof(uint32_t));
    size_t tl = ld;
    if (q == 0) { tl = 0; }
    else A3067_mul(A3067_T, &tl, q);
    memcpy(A3067_R, A3067_N, ln * sizeof(uint32_t));
    A3067_sub(A3067_R, ln, A3067_T, tl);
    while (A3067_cmp(A3067_R, ln, A3067_D, ld) >= 0) {
        A3067_sub(A3067_R, ln, A3067_D, ld);
        q++;
    }
    /* double R */
    uint32_t c = 0;
    for (size_t i = 0; i < ln; i++) {
        uint32_t v = A3067_R[i];
        A3067_R[i] = (v << 1) | c;
        c = v >> 31;
    }
    A3067_R[ln] = c;
    int cmpr = A3067_cmp(A3067_R, ln + 1, A3067_D, ld);
    uint64_t t = prev + q;
    if (cmpr > 0 || (cmpr == 0 && (t & 1))) t++;

    if (A3067_cnt >= A3067_tcap) {
        A3067_tcap = A3067_tcap ? A3067_tcap * 2 : 64;
        A3067_terms = (uint64_t *)realloc(A3067_terms, A3067_tcap * sizeof(uint64_t));
    }
    A3067_terms[A3067_cnt++] = t;
    A3067_ensure((A3067_ln > A3067_ld ? A3067_ln : A3067_ld) + 4);
    A3067_mul(A3067_N, &A3067_ln, t);
    A3067_mul(A3067_D, &A3067_ld, t - 1);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (A3067_cnt == 0) {
        A3067_ensure(16);
        A3067_tcap = 64;
        A3067_terms = (uint64_t *)malloc(A3067_tcap * sizeof(uint64_t));
        A3067_terms[0] = 2;
        A3067_cnt = 1;
        A3067_N[0] = 2; A3067_ln = 1;
        A3067_D[0] = 1; A3067_ld = 1;
    }
    while (A3067_cnt < n) A3067_extend();
    return (seq_t)A3067_terms[n - 1];
}
