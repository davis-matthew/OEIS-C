/*
 * A001425  Number of commutative groupoids with n elements.
 * https://oeis.org/A001425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1*s_1+2*s_2+...=n}
 *            fixA[s_1,s_2,...]/(1^s_1*s_1!*2^s_2*s_2!*...), fixA = Product_i
 *            D(i)^(s_i*ceil(i/2) + i*binomial(s_i,2)) * [i even] D(i/2)^s_i *
 *            Product_{i<j} D(lcm(i,j))^(gcd(i,j)*s_i*s_j), D(L) = Sum_{d|L}
 *            d*s_d. Computed as (Sum_{cycle types} classsize * fixA) / n!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   a1425_gcd, a1425_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a1425_gcd.h"
#include "../utils/a1425_pw.h"

static idx_t a1425_parts[128];
static idx_t a1425_cnt[128];
static int a1425_k;

static uint64_t a1425_D(uint64_t L) {
    uint64_t s = 0;
    for (int t = 0; t < a1425_k; t++)
        if (L % a1425_parts[t] == 0) s += a1425_parts[t] * a1425_cnt[t];
    return s;
}

static seq_t a1425_term(idx_t n) {
    seq_t fix = 1;
    for (int t = 0; t < a1425_k; t++) {
        uint64_t i = a1425_parts[t], c = a1425_cnt[t];
        uint64_t d = a1425_D(i);
        uint64_t e = (i % 2 == 1) ? (i * c * c + c) / 2 : i * c * c / 2;
        if (e > 0) {
            if (d == 0) return 0;
            fix *= a1425_pw((seq_t)d, e);
        }
        if (i % 2 == 0) {
            uint64_t d2 = a1425_D(i / 2);
            if (d2 == 0) return 0;
            fix *= a1425_pw((seq_t)d2, c);
        }
    }
    for (int t = 0; t < a1425_k; t++) {
        for (int u = t + 1; u < a1425_k; u++) {
            uint64_t i = a1425_parts[t], j = a1425_parts[u];
            uint64_t g = a1425_gcd(i, j);
            uint64_t l = i / g * j;
            uint64_t d = a1425_D(l);
            uint64_t e = g * a1425_cnt[t] * a1425_cnt[u];
            if (d == 0) return 0;
            fix *= a1425_pw((seq_t)d, e);
        }
    }
    /* class size = n! / prod(i^s_i * s_i!) */
    seq_t cls = 1;
    for (idx_t m = 2; m <= n; m++) cls *= (seq_t)m;
    for (int t = 0; t < a1425_k; t++) {
        uint64_t i = a1425_parts[t], c = a1425_cnt[t];
        for (uint64_t q = 0; q < c; q++) cls /= (seq_t)i;
        for (uint64_t q = 2; q <= c; q++) cls /= (seq_t)q;
    }
    return cls * fix;
}

static void a1425_rec(idx_t n, idx_t rem, idx_t maxp, seq_t *sum) {
    if (rem == 0) {
        *sum += a1425_term(n);
        return;
    }
    for (idx_t p = (maxp < rem ? maxp : rem); p >= 1; p--) {
        for (idx_t c = 1; c * p <= rem; c++) {
            a1425_parts[a1425_k] = p;
            a1425_cnt[a1425_k] = c;
            a1425_k++;
            a1425_rec(n, rem - c * p, p - 1, sum);
            a1425_k--;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t sum = 0;
    a1425_k = 0;
    a1425_rec(n, n, n, &sum);
    seq_t f = 1;
    for (idx_t m = 2; m <= n; m++) f *= (seq_t)m;
    return sum / f;
}
