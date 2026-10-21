/*
 * A091220  Number of divisors of the n-th GF(2)[X]-polynomial.
 * https://oeis.org/A091220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   poly_degree, poly_div_once, poly_divides
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the number of monic divisors of the GF(2)[X]‑polynomial whose
 * coefficients are the binary digits of n (least significant bit is the
 * constant term).  For example, n=10 (binary 1010) represents
 * X³ + X and its divisors are 1, X²+X, X³+X, and itself, so a(10)=4.  */
/* This implementation precomputes the list of all monic irreducible
 * polynomials of degree ≤ 14 (sufficient for n ≤ 10000) and then
 * uses repeated polynomial division to factor each f(n). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/poly_degree.h"
#include "../utils/poly_div_once.h"
#include "../utils/poly_divides.h"

/* ----- helper: polynomial arithmetic over GF(2) using 64‑bit bitsets ----- */

/* ----- precomputation of all monic irreducible polynomials up to degree 14 ----- */
#define MAXDEG 14
#define MAXIRR_PER_DEG 2000   /* enough for all degrees up to 14 */

static bool irr_init = false;
static uint64_t irr[MAXDEG + 1][MAXIRR_PER_DEG];
static uint16_t irr_cnt[MAXDEG + 1];

static void init_irreducibles(void)
{
    if (irr_init) return;
    for (int d = 1; d <= MAXDEG; ++d) irr_cnt[d] = 0;

    for (int d = 1; d <= MAXDEG; ++d) {
        uint64_t limit = (uint64_t)1 << d;
        for (uint64_t i = 0; i < limit; ++i) {
            uint64_t poly = ((uint64_t)1 << d) | i;   /* monic polynomial of degree d */
            bool is_irr = true;
            /* test divisibility by smaller irreducibles */
            for (int d2 = 1; d2 <= d / 2 && is_irr; ++d2) {
                for (uint16_t j = 0; j < irr_cnt[d2]; ++j) {
                    if (poly_divides(poly, irr[d2][j])) {
                        is_irr = false;
                        break;
                    }
                }
            }
            if (is_irr) {
                irr[d][irr_cnt[d]++] = poly;
            }
        }
    }
    irr_init = true;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    init_irreducibles();

    uint64_t f = (uint64_t)n;            /* polynomial bits */
    seq_t res = 1;

    int d = poly_degree(f);
    if (d == -1) return res;            /* n == 0, should not happen */

    for (int deg = 1; deg <= d / 2; ++deg) {
        for (uint16_t k = 0; k < irr_cnt[deg]; ++k) {
            uint64_t g = irr[deg][k];
            if (!poly_divides(f, g)) continue;
            int exp = 0;
            while (poly_divides(f, g)) {
                f = poly_div_once(f, g);
                ++exp;
            }
            if (exp > 0) res *= (seq_t)(exp + 1);
            if (f == 1) break;
        }
        if (f == 1) break;
    }

    /* remaining factor (if any) is irreducible with exponent 1 */
    if (f != 1) res *= (seq_t)2;

    return res;
}
