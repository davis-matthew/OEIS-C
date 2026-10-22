/*
 * A004606  Expansion of Pi in base 7.
 * https://oeis.org/A004606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(n) = n-th digit of Pi in base 7, with Pi = 16*arctan(1/5) -
 *            4*arctan(1/239) (Machin) in multiprecision binary fixed point,
 *            then converted to base 7
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fp_add, fp_arctan_inv, fp_div, fp_iszero, fp_mul, fp_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/fp_add.h"
#include "../utils/fp_arctan_inv.h"
#include "../utils/fp_div.h"
#include "../utils/fp_iszero.h"
#include "../utils/fp_mul.h"
#include "../utils/fp_sub.h"

static unsigned char *pi7_digits = NULL;
static idx_t pi7_count = 0;

static void pi7_compute(idx_t N) {
    size_t bits = (size_t)N * 3 + 96;
    size_t L = bits / 32 + 3;
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)calloc(L, sizeof(uint32_t));
    idx_t i;
    fp_arctan_inv(p, L, 5);
    fp_mul(p, L, 16);
    fp_arctan_inv(q, L, 239);
    fp_mul(q, L, 4);
    fp_sub(p, q, L);
    free(pi7_digits);
    pi7_digits = (unsigned char *)malloc((size_t)N);
    pi7_digits[0] = (unsigned char)p[0];
    for (i = 1; i < N; i++) {
        p[0] = 0;
        fp_mul(p, L, 7);
        pi7_digits[i] = (unsigned char)p[0];
    }
    free(p);
    free(q);
    pi7_count = N;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > pi7_count) {
        idx_t N = pi7_count * 2;
        if (N < n) N = n;
        if (N < 64) N = 64;
        pi7_compute(N);
    }
    return (seq_t)pi7_digits[n - 1];
}
