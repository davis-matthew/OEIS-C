/*
 * A007969  Rectangular numbers.
 * https://oeis.org/A007969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over m >= 2. Nonsquare m is rectangular iff the
 *            fundamental solution (x,y) of x^2 - m*y^2 = 1 has x odd and y
 *            even. Only the parities of x and y are needed, so they come from
 *            the continued fraction convergents of sqrt(m) taken mod 2
 * Range:     u8 n <= 163, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t isqrt_u64(uint64_t x) {
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

static bool is_rect(uint64_t m) {
    uint64_t a0 = isqrt_u64(m);
    if (a0 * a0 == m) return false;
    /* period length L */
    uint64_t mk = 0, dk = 1, ak = a0, L = 0;
    while (ak != 2 * a0) {
        mk = dk * ak - mk;
        dk = (m - mk * mk) / dk;
        ak = (a0 + mk) / dk;
        L++;
    }
    uint64_t N = (L % 2 == 0) ? L - 1 : 2 * L - 1;
    /* convergents mod 2 */
    unsigned p1 = 1, p2 = 0, q1 = 0, q2 = 1;
    mk = 0; dk = 1; ak = a0;
    for (uint64_t k = 0;; k++) {
        unsigned am = (unsigned)(ak & 1);
        unsigned p = (am * p1 + p2) & 1;
        unsigned q = (am * q1 + q2) & 1;
        p2 = p1; p1 = p;
        q2 = q1; q1 = q;
        if (k == N) break;
        mk = dk * ak - mk;
        dk = (m - mk * mk) / dk;
        ak = (a0 + mk) / dk;
    }
    return p1 == 1 && q1 == 0;
}

static uint64_t *terms = NULL;
static idx_t nterms = 0, cap = 0;
static uint64_t next_m = 2;

seq_t a(idx_t n) {
    while (nterms < n) {
        if (is_rect(next_m)) {
            if (nterms == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, (size_t)cap * sizeof(uint64_t));
            }
            terms[nterms++] = next_m;
        }
        next_m++;
    }
    return (seq_t)terms[n - 1];
}
