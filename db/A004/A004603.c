/*
 * A004603  Expansion of Pi in base 4.
 * https://oeis.org/A004603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(1) = 3, a(n) = (n-1)-th base-4 digit of Pi after the point; Pi
 *            = 16*arctan(1/5) - 4*arctan(1/239) (Machin) in multiprecision
 *            fixed point
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mp_add, mp_div, mp_iszero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mp_add.h"
#include "../utils/mp_div.h"
#include "../utils/mp_iszero.h"

#define PI_GUARD 3

static uint32_t *pi_limbs = NULL;
static idx_t pi_nlimbs = 0;

static void mp_sub(uint32_t *x, const uint32_t *y, idx_t len) {
    uint64_t borrow = 0;
    idx_t i = len;
    while (i > 0) {
        i--;
        uint64_t yi = (uint64_t)y[i] + borrow;
        if ((uint64_t)x[i] >= yi) {
            x[i] = (uint32_t)((uint64_t)x[i] - yi);
            borrow = 0;
        } else {
            x[i] = (uint32_t)(((uint64_t)1 << 32) + x[i] - yi);
            borrow = 1;
        }
    }
}

/* acc += sign * mult * arctan(1/m) */
static void mp_atan_acc(uint32_t *acc, idx_t len, uint32_t mult, uint32_t m, int sign) {
    uint32_t *term = (uint32_t *)calloc(len, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)malloc(len * sizeof(uint32_t));
    uint32_t m2 = m * m;
    uint64_t k = 0;
    term[0] = mult;
    mp_div(term, len, m);
    while (!mp_iszero(term, len)) {
        memcpy(t, term, len * sizeof(uint32_t));
        mp_div(t, len, (uint32_t)(2 * k + 1));
        if (((k & 1) == 0) == (sign > 0))
            mp_add(acc, t, len);
        else
            mp_sub(acc, t, len);
        mp_div(term, len, m2);
        k++;
    }
    free(term);
    free(t);
}

static void compute_pi(idx_t nlimbs) {
    uint32_t *p = (uint32_t *)calloc(nlimbs, sizeof(uint32_t));
    mp_atan_acc(p, nlimbs, 16, 5, 1);
    mp_atan_acc(p, nlimbs, 4, 239, -1);
    free(pi_limbs);
    pi_limbs = p;
    pi_nlimbs = nlimbs;
}

seq_t a(idx_t n) {
    idx_t d, need, avail;
    if (n <= 1) return (seq_t)3;
    d = n - 1;
    need = 1 + (d - 1) / 16 + 1;
    avail = pi_nlimbs > PI_GUARD + 1 ? pi_nlimbs - PI_GUARD : 0;
    if (pi_limbs == NULL || avail < need) {
        idx_t nl = need * 2 + PI_GUARD + 16;
        compute_pi(nl);
    }
    {
        uint32_t limb = pi_limbs[1 + (d - 1) / 16];
        unsigned sh = (unsigned)(30 - 2 * ((d - 1) % 16));
        return (seq_t)((limb >> sh) & 3u);
    }
}
