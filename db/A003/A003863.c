/*
 * A003863  Degrees of irreducible representations of alternating group A_8.
 * https://oeis.org/A003863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   degrees of the irreducible characters of A_8, obtained from the
 *            S_8 irreducibles: for each partition of 8 compute f(lambda) =
 *            8!/prod(hook lengths); a pair {lambda, lambda'} with lambda !=
 *            lambda' restricts to one irreducible of degree f, and a
 *            self-conjugate lambda splits into two of degree f/2....
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   a003863_cmp, a003863_conj
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a003863_cmp.h"
#include "../utils/a003863_conj.h"

#define A003863_N 8

static long long a003863_deg[64];
static int a003863_cnt = 0;
static bool a003863_done = false;

static long long a003863_dim(const int *p, int len)
{
    int c[A003863_N + 1], clen, i, j;
    long long num = 1, den = 1;
    a003863_conj(p, len, c, &clen);
    for (i = 2; i <= A003863_N; i++) num *= i;
    for (i = 0; i < len; i++)
        for (j = 0; j < p[i]; j++)
            den *= (long long)(p[i] - j - 1) + (c[j] - i - 1) + 1;
    return num / den;
}

static void a003863_visit(const int *p, int len)
{
    int c[A003863_N + 1], clen, i, cmp = 0;
    long long f;
    a003863_conj(p, len, c, &clen);
    for (i = 0; i < len && i < clen; i++) {
        if (p[i] != c[i]) { cmp = (p[i] < c[i]) ? -1 : 1; break; }
    }
    if (cmp == 0 && len != clen) cmp = (len < clen) ? -1 : 1;
    if (cmp > 0) return;            /* count each conjugate pair once */
    f = a003863_dim(p, len);
    if (cmp == 0) {
        a003863_deg[a003863_cnt++] = f / 2;
        a003863_deg[a003863_cnt++] = f / 2;
    } else {
        a003863_deg[a003863_cnt++] = f;
    }
}

static void a003863_gen(int *p, int len, int rem, int maxpart)
{
    int k;
    if (rem == 0) { a003863_visit(p, len); return; }
    for (k = (rem < maxpart ? rem : maxpart); k >= 1; k--) {
        p[len] = k;
        a003863_gen(p, len + 1, rem - k, k);
    }
}

static void a003863_init(void)
{
    int p[A003863_N + 1];
    if (a003863_done) return;
    a003863_cnt = 0;
    a003863_gen(p, 0, A003863_N, A003863_N);
    qsort(a003863_deg, (size_t)a003863_cnt, sizeof(long long), a003863_cmp);
    a003863_done = true;
}

seq_t a(idx_t n)
{
    a003863_init();
    if (n < 1 || n > (idx_t)a003863_cnt) return (seq_t)0;
    return (seq_t)a003863_deg[n - 1];
}
