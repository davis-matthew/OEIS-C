/*
 * A000827  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(6*n!)) * Sum_{sigma in S_n} Sum_{tau in S_3}
 *            Prod_{cycles of sigma on {0,1}^n, length L} 2^{cyc(tau^L)}
 *            (Burnside: 3-output Boolean maps of n inputs, S_n on inputs x
 *            S_3 on outputs). For each cycle type of sigma with C cycles on
 *            {0,1}^n, O of odd length, T of length divisible by 3: con...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   gcd_u64, pow2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"
#include "../utils/pow2_u64.h"

static void leaf827(const int *parts, int k, idx_t n, seq_t *S)
{
    uint64_t m = 1;
    int i, j;
    for (i = 0; i < k; i++) m = m / gcd_u64(m, (uint64_t)parts[i]) * (uint64_t)parts[i];

    uint64_t nd = 0, d;
    for (d = 1; d <= m; d++) if (m % d == 0) nd++;
    uint64_t *divs = (uint64_t *)malloc(sizeof(uint64_t) * nd);
    uint64_t *E = (uint64_t *)malloc(sizeof(uint64_t) * nd);
    nd = 0;
    for (d = 1; d <= m; d++) if (m % d == 0) divs[nd++] = d;

    uint64_t C = 0, O = 0, T = 0;
    for (j = 0; j < (int)nd; j++) {
        uint64_t c = 0;
        for (i = 0; i < k; i++) c += gcd_u64((uint64_t)parts[i], divs[j]);
        uint64_t e = (uint64_t)1 << c;
        for (i = 0; i < j; i++)
            if (divs[j] % divs[i] == 0) e -= E[i];
        E[j] = e;
        uint64_t cyc = e / divs[j];
        C += cyc;
        if (divs[j] % 2 == 1) O += cyc;
        if (divs[j] % 3 == 0) T += cyc;
    }
    free(divs);
    free(E);

    seq_t F = pow2_u64(3 * C);
    F += (seq_t)3 * pow2_u64(2 * O + 3 * (C - O));
    F += (seq_t)2 * pow2_u64(3 * T + (C - T));

    /* class size n! / prod(i^{m_i} m_i!) */
    seq_t cs = 1;
    idx_t t;
    for (t = 2; t <= n; t++) cs *= (seq_t)t;
    i = 0;
    while (i < k) {
        int p = parts[i], mult = 0, r;
        while (i < k && parts[i] == p) { i++; mult++; }
        for (r = 1; r <= mult; r++) {
            cs /= (seq_t)p;
            cs /= (seq_t)r;
        }
    }
    *S += cs * F;
}

static void rec827(int rem, int maxp, int *parts, int k, idx_t n, seq_t *S)
{
    if (rem == 0) { leaf827(parts, k, n, S); return; }
    int p;
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        parts[k] = p;
        rec827(rem - p, p, parts, k + 1, n, S);
    }
}

seq_t a(idx_t n)
{
    int *parts = (int *)malloc(sizeof(int) * (size_t)(n + 1));
    seq_t S = 0;
    rec827((int)n, (int)n, parts, 0, n, &S);
    free(parts);
    S /= 6;
    idx_t t;
    for (t = 2; t <= n; t++) S /= (seq_t)t;
    return S;
}
