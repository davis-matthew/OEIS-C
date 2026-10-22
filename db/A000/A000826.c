/*
 * A000826  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2*n!)) * Sum_{sigma in S_n} (4^c(sigma) +
 *            4^e(sigma)*2^o(sigma)), where sigma acts on the 2^n points of
 *            {0,1}^n, c = number of its cycles, e = number of even-length
 *            cycles, o = number of odd-length cycles (Burnside, S_n on inputs
 *            x S_2 swapping the two output bits). Summed over cycle t...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a826_div, a826_gcd, a826_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a826_div.h"
#include "../utils/a826_gcd.h"
#include "../utils/a826_mul.h"

static uint32_t *a826_S;
static size_t a826_SL;
static uint32_t *a826_cnt;
static size_t a826_cl;
static int a826_parts[256];
static int a826_N;

static void a826_add_shifted(const uint32_t *x, size_t xl, uint64_t sh) {
    size_t w = (size_t)(sh / 32);
    unsigned bit = (unsigned)(sh % 32);
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < xl && w + i < a826_SL; i++) {
        uint64_t t = (uint64_t)x[i] << bit;
        uint64_t cur = (uint64_t)a826_S[w + i] + (t & 0xffffffffULL) + carry;
        a826_S[w + i] = (uint32_t)cur;
        carry = (cur >> 32) + (t >> 32);
    }
    i += w;
    while (carry && i < a826_SL) {
        uint64_t cur = (uint64_t)a826_S[i] + carry;
        a826_S[i] = (uint32_t)cur;
        carry = cur >> 32;
        i++;
    }
}

/* Number of cycles of sigma restricted to Fix(sigma^M), by Burnside over <sigma> mod M. */
static uint64_t a826_cycles(int k, uint64_t M) {
    unsigned __int128 sum = 0;
    uint64_t j;
    int i;
    for (j = 0; j < M; j++) {
        uint64_t g = a826_gcd(j, M);
        unsigned ex = 0;
        for (i = 0; i < k; i++)
            ex += (unsigned)a826_gcd((uint64_t)a826_parts[i], g);
        sum += (unsigned __int128)1 << ex;
    }
    return (uint64_t)(sum / M);
}

static void a826_process(int k) {
    uint64_t m = 1, o;
    int i, j;
    for (i = 0; i < k; i++)
        m = m / a826_gcd(m, (uint64_t)a826_parts[i]) * (uint64_t)a826_parts[i];
    o = m;
    while ((o & 1) == 0) o >>= 1;
    uint64_t c = a826_cycles(k, m);
    uint64_t codd = a826_cycles(k, o);

    memset(a826_cnt, 0, a826_cl * sizeof(uint32_t));
    a826_cnt[0] = 1;
    for (i = 2; i <= a826_N; i++) a826_mul(a826_cnt, a826_cl, (uint32_t)i);
    i = 0;
    while (i < k) {
        j = i;
        while (j < k && a826_parts[j] == a826_parts[i]) j++;
        int mult = j - i, t;
        for (t = 0; t < mult; t++) a826_div(a826_cnt, a826_cl, (uint32_t)a826_parts[i]);
        for (t = 2; t <= mult; t++) a826_div(a826_cnt, a826_cl, (uint32_t)t);
        i = j;
    }
    a826_add_shifted(a826_cnt, a826_cl, 2 * c);
    a826_add_shifted(a826_cnt, a826_cl, 2 * c - codd);
}

static void a826_gen(int rem, int maxp, int k) {
    int p;
    if (rem == 0) { a826_process(k); return; }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        a826_parts[k] = p;
        a826_gen(rem - p, p, k + 1);
    }
}

seq_t a(idx_t n) {
    int N = (int)n, i, b;
    a826_N = N;
    a826_cl = (size_t)N * 7 / 32 + 4;
    a826_SL = ((((size_t)1) << (N + 1)) + 64) / 32 + a826_cl + 4;
    a826_S = (uint32_t *)calloc(a826_SL, sizeof(uint32_t));
    a826_cnt = (uint32_t *)calloc(a826_cl, sizeof(uint32_t));

    a826_gen(N, N, 0);

    a826_div(a826_S, a826_SL, 2);
    for (i = 2; i <= N; i++) a826_div(a826_S, a826_SL, (uint32_t)i);

    seq_t r = 0;
    size_t top = a826_SL;
    while (top > 0 && a826_S[top - 1] == 0) top--;
    while (top > 0) {
        top--;
        uint32_t limb = a826_S[top];
        for (b = 3; b >= 0; b--) {
            r = r * (seq_t)256;
            r = r + (seq_t)((limb >> (8 * b)) & 255u);
        }
    }
    free(a826_S);
    free(a826_cnt);
    return r;
}
