/*
 * A000815  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of orbits of maps {0,1}^n -> {8 colours} (3 output
 *            bits) under the hyperoctahedral group B_n (input permutations
 *            and complementations); by Burnside, a(n) = (1/(2^n n!)) * Sum_{g
 *            in B_n} 8^cyc(g), summed over signed cycle types of g
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a815_gcd, a815_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a815_gcd.h"
#include "../utils/a815_pw.h"

static uint64_t *a815_mp, *a815_mm;
static long long a815_n;
static seq_t a815_acc, a815_total;

static void a815_leaf(void)
{
    long long n = a815_n, k;
    uint64_t m = 1, j, s = 0, cyc, i;
    seq_t den = 1, size;
    for (k = 1; k <= n; k++) {
        if (a815_mp[k]) m = m / a815_gcd(m, (uint64_t)k) * (uint64_t)k;
        if (a815_mm[k]) m = m / a815_gcd(m, 2 * (uint64_t)k) * (2 * (uint64_t)k);
    }
    for (j = 0; j < m; j++) {
        uint64_t e = 0;
        int ok = 1;
        for (k = 1; k <= n && ok; k++) {
            if (a815_mp[k]) e += a815_mp[k] * a815_gcd(j, (uint64_t)k);
            if (a815_mm[k]) {
                uint64_t g = a815_gcd(j, (uint64_t)k);
                if ((((uint64_t)k / g) & 1) && ((j / g) % 2 == 0))
                    e += a815_mm[k] * g;
                else
                    ok = 0;
            }
        }
        if (ok) s += (uint64_t)1 << e;
    }
    cyc = s / m;
    for (k = 1; k <= n; k++) {
        for (i = 0; i < a815_mp[k] + a815_mm[k]; i++) den *= (seq_t)(2 * k);
        for (i = 2; i <= a815_mp[k]; i++) den *= (seq_t)i;
        for (i = 2; i <= a815_mm[k]; i++) den *= (seq_t)i;
    }
    size = a815_total / den;
    a815_acc += a815_pw((seq_t)8, cyc) * size;
}

static void a815_rec(long long k, long long rem)
{
    long long t, p;
    if (rem == 0) { a815_leaf(); return; }
    if (k > rem) return;
    for (t = 0; t * k <= rem; t++) {
        for (p = 0; p <= t; p++) {
            a815_mp[k] = (uint64_t)p;
            a815_mm[k] = (uint64_t)(t - p);
            a815_rec(k + 1, rem - t * k);
        }
    }
    a815_mp[k] = 0;
    a815_mm[k] = 0;
}

seq_t a(idx_t n)
{
    long long nn = (long long)n, i;
    seq_t res;
    a815_n = nn;
    a815_mp = (uint64_t *)calloc((size_t)(nn + 2), sizeof(uint64_t));
    a815_mm = (uint64_t *)calloc((size_t)(nn + 2), sizeof(uint64_t));
    a815_total = 1;
    for (i = 1; i <= nn; i++) a815_total *= (seq_t)(2 * i);
    a815_acc = 0;
    a815_rec(1, nn);
    res = a815_acc / a815_total;
    free(a815_mp);
    free(a815_mm);
    return res;
}
