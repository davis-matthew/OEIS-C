/*
 * A001377  Number of relations with 4 arguments on n nodes.
 * https://oeis.org/A001377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{partitions lambda of n} 2^c(lambda) / z(lambda),
 *            where z(lambda) = Prod_i i^m_i * m_i! and c(lambda) =
 *            Sum_{i,j,k,l} m_i m_j m_k m_l * i*j*k*l / lcm(i,j,k,l) (Burnside
 *            count of 4-ary relations on n unlabeled nodes; cycles of S_n
 *            acting on n^4 tuples)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   a001377_fixed, a001377_gcd, a001377_inv, a001377_pow2,
 *            a677_from_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a677_from_u128.h"
#include "../utils/a001377_fixed.h"
#include "../utils/a001377_gcd.h"
#include "../utils/a001377_inv.h"
#include "../utils/a001377_pow2.h"

static uint64_t *a001377_L = NULL;
static uint64_t *a001377_M = NULL;
static int a001377_isfixed = 0;
static seq_t a001377_nf;
static seq_t a001377_sum;
static unsigned __int128 a001377_sum128;

static uint64_t a001377_cycles(int k) {
    uint64_t c = 0;
    int i, j, s, t;
    for (i = 0; i < k; i++) {
        uint64_t li = a001377_L[i], mi = a001377_M[i];
        for (j = 0; j < k; j++) {
            uint64_t lj = a001377_L[j], mj = a001377_M[j];
            uint64_t l2 = li / a001377_gcd(li, lj) * lj;
            for (s = 0; s < k; s++) {
                uint64_t ls = a001377_L[s], ms = a001377_M[s];
                uint64_t l3 = l2 / a001377_gcd(l2, ls) * ls;
                for (t = 0; t < k; t++) {
                    uint64_t lt = a001377_L[t], mt = a001377_M[t];
                    uint64_t l4 = l3 / a001377_gcd(l3, lt) * lt;
                    c += mi * mj * ms * mt * (li * lj * ls * lt / l4);
                }
            }
        }
    }
    return c;
}

static void a001377_leaf(int k) {
    uint64_t c = a001377_cycles(k);
    int i;
    uint64_t j;
    if (a001377_isfixed) {
        uint64_t f = 0;
        unsigned __int128 oddz = 1;
        for (i = 0; i < k; i++) {
            uint64_t x = a001377_L[i];
            while ((x & 1) == 0) { x >>= 1; f += a001377_M[i]; }
            for (j = 0; j < a001377_M[i]; j++) oddz *= x;
            for (j = 2; j <= a001377_M[i]; j++) {
                uint64_t y = j;
                while ((y & 1) == 0) { y >>= 1; f++; }
                oddz *= y;
            }
        }
        if (c >= f && c - f < 128) {
            unsigned __int128 term = (unsigned __int128)1 << (c - f);
            a001377_sum128 += term * a001377_inv(oddz);
        }
    } else {
        seq_t z = 1;
        for (i = 0; i < k; i++) {
            for (j = 0; j < a001377_M[i]; j++) z *= (seq_t)a001377_L[i];
            for (j = 2; j <= a001377_M[i]; j++) z *= (seq_t)j;
        }
        a001377_sum += a001377_nf / z * a001377_pow2(c);
    }
}

static void a001377_rec(uint64_t rem, uint64_t maxp, int k) {
    uint64_t p, m;
    if (rem == 0) { a001377_leaf(k); return; }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        for (m = 1; m * p <= rem; m++) {
            a001377_L[k] = p;
            a001377_M[k] = m;
            a001377_rec(rem - p * m, p - 1, k + 1);
        }
    }
}

seq_t a(idx_t n) {
    idx_t i;
    seq_t res;
    a001377_isfixed = a001377_fixed();
    if (a001377_isfixed && n > 3) return 0; /* a(n) >= 2^(n^4)/n! exceeds 128 bits */
    a001377_L = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    a001377_M = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    a001377_nf = 1;
    if (!a001377_isfixed)
        for (i = 2; i <= n; i++) a001377_nf *= (seq_t)i;
    a001377_sum = 0;
    a001377_sum128 = 0;
    a001377_rec((uint64_t)n, (uint64_t)n, 0);
    if (a001377_isfixed) res = a677_from_u128(a001377_sum128);
    else res = a001377_sum / a001377_nf;
    free(a001377_L);
    free(a001377_M);
    a001377_L = NULL;
    a001377_M = NULL;
    return res;
}
