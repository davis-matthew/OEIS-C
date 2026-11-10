/*
 * A055889  CIK transform of partition triangle A008284.
 * https://oeis.org/A055889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   CIK transform of the bivariate g.f. A(x,y) = Sum T(n,k) x^n y^k,
 *            T = A008284. C = 1/(1-A), N(n,k) = [x^n y^k] x*dA/dx * C (= n *
 *            [x^n y^k] -log(1-A)), T(n,k) of A055889 = (1/n) * Sum_{d |
 *            gcd(n,k)} phi(d) * N(n/d, k/d). Rows read by rows, 1 <= k <= n
 * Range:     u8 n <= 25, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A055889_B = NULL;
static int A055889_R = 0;      /* rows computed; B has (R+1)*(R+1) entries */

static int A055889_phi(int m)
{
    int r = m, p;
    for (p = 2; p * p <= m; p++)
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            r -= r / p;
        }
    if (m > 1) r -= r / m;
    return r;
}

static void A055889_build(int R)
{
    int W = R + 1;
    seq_t *T = mem_new(seq_t, (size_t)W * W);
    seq_t *C = mem_new(seq_t, (size_t)W * W);
    seq_t *N = mem_new(seq_t, (size_t)W * W);
    seq_t *B = mem_new(seq_t, (size_t)W * W);
    int i, j, s, l, n, k, d;

    /* T[i][j] = partitions of i into exactly j parts (A008284) */
    T[0] = 1;
    for (i = 1; i <= R; i++)
        for (j = 1; j <= i; j++) {
            seq_t v = T[(i - 1) * W + (j - 1)];
            if (i - j >= j) v += T[(i - j) * W + j];
            T[i * W + j] = v;
        }
    T[0] = 0;   /* A has no constant term */

    /* C = 1/(1-A) */
    C[0] = 1;
    for (n = 1; n <= R; n++)
        for (k = 1; k <= n; k++) {
            seq_t sum = 0;
            for (s = 1; s <= n; s++)
                for (l = 1; l <= s && l <= k; l++) {
                    if (n - s < k - l) continue;
                    sum += T[s * W + l] * C[(n - s) * W + (k - l)];
                }
            C[n * W + k] = sum;
        }

    /* N = x A_x * C */
    for (n = 1; n <= R; n++)
        for (k = 1; k <= n; k++) {
            seq_t sum = 0;
            for (s = 1; s <= n; s++)
                for (l = 1; l <= s && l <= k; l++) {
                    if (n - s < k - l) continue;
                    sum += T[s * W + l] * (seq_t)s * C[(n - s) * W + (k - l)];
                }
            N[n * W + k] = sum;
        }

    for (n = 1; n <= R; n++)
        for (k = 1; k <= n; k++) {
            seq_t sum = 0;
            for (d = 1; d <= k; d++)
                if (n % d == 0 && k % d == 0)
                    sum += (seq_t)A055889_phi(d) * N[(n / d) * W + (k / d)];
            B[n * W + k] = sum / (seq_t)n;
        }

    mem_free(T);
    mem_free(C);
    mem_free(N);
    if (A055889_B) mem_free(A055889_B);
    A055889_B = B;
    A055889_R = R;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) r++;
    idx_t c = n - r * (r - 1) / 2;
    if (A055889_R < (int)r) {
        int R = (int)r;
        if (R < 2 * A055889_R) R = 2 * A055889_R;
        if (R < 16) R = 16;
        A055889_build(R);
    }
    return A055889_B[(size_t)r * (A055889_R + 1) + (size_t)c];
}
