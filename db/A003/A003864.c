/*
 * A003864  Degrees of irreducible representations of alternating group A_9.
 * https://oeis.org/A003864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   finite list (fini, full). Enumerate the partitions lambda of 9
 *            and get dim chi_lambda of S_9 = 9!/prod(hook lengths). Each pair
 *            {lambda, lambda'} with lambda != lambda' restricts to one
 *            irreducible of A_9 of the same degree. A self-conjugate lambda
 *            splits into two irreducibles of degree dim/2. So...
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   a3864_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/a3864_cmp.h"

#define A3864_N 9

static long long a3864_deg[64];
static int a3864_cnt = 0;
static int a3864_ready = 0;

static long long a3864_dim(const int *p, int len) {
    int conj[A3864_N + 1];
    int i, j;
    long long num = 1, den = 1;
    memset(conj, 0, sizeof(conj));
    for (i = 0; i < len; i++)
        for (j = 0; j < p[i]; j++) conj[j]++;
    for (i = 2; i <= A3864_N; i++) num *= i;
    for (i = 0; i < len; i++)
        for (j = 0; j < p[i]; j++)
            den *= (long long)(p[i] - j - 1 + conj[j] - i - 1 + 1);
    return num / den;
}

static void a3864_process(const int *p, int len) {
    int conj[A3864_N + 1];
    int i, j, clen = p[0], cmp = 0;
    long long d;
    memset(conj, 0, sizeof(conj));
    for (i = 0; i < len; i++)
        for (j = 0; j < p[i]; j++) conj[j]++;
    for (i = 0; i < A3864_N && cmp == 0; i++) {
        int x = i < len ? p[i] : 0;
        int y = i < clen ? conj[i] : 0;
        if (x > y) cmp = 1;
        else if (x < y) cmp = -1;
    }
    d = a3864_dim(p, len);
    if (cmp == 0) {
        a3864_deg[a3864_cnt++] = d / 2;
        a3864_deg[a3864_cnt++] = d / 2;
    } else if (cmp > 0) {
        a3864_deg[a3864_cnt++] = d;
    }
}

static void a3864_gen(int *p, int len, int rem, int maxpart) {
    int k;
    if (rem == 0) { a3864_process(p, len); return; }
    for (k = (rem < maxpart ? rem : maxpart); k >= 1; k--) {
        p[len] = k;
        a3864_gen(p, len + 1, rem - k, k);
    }
}

seq_t a(idx_t n) {
    if (!a3864_ready) {
        int p[A3864_N + 1];
        a3864_gen(p, 0, A3864_N, A3864_N);
        qsort(a3864_deg, (size_t)a3864_cnt, sizeof(long long), a3864_cmp);
        a3864_ready = 1;
    }
    if (n < 1 || n > (idx_t)a3864_cnt) return (seq_t)0;
    return (seq_t)a3864_deg[n - 1];
}
