/*
 * A003867  Degrees of irreducible representations of alternating group A_12.
 * https://oeis.org/A003867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   enumerate the partitions L of 12 and get each degree f_L from
 *            the hook length formula, f_L = 12!/prod(hooks). If L != L'
 *            (conjugate partition), the pair {L, L'} restricts to one
 *            irreducible of A_12 with degree f_L. If L = L', it splits into
 *            two irreducibles, each of degree f_L/2. a(n) is the n-th...
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned long long a003867_degs[64];
static int a003867_cnt = -1;
static int a003867_part[13];

static void a003867_process(int len) {
    int c[13];
    int clen = a003867_part[0];
    int i, j, cmp = 0;
    unsigned long long prod = 1;
    for (j = 0; j < clen; j++) {
        c[j] = 0;
        for (i = 0; i < len; i++)
            if (a003867_part[i] > j) c[j]++;
    }
    for (i = 0; i < 13 && cmp == 0; i++) {
        int x = i < len ? a003867_part[i] : 0;
        int y = i < clen ? c[i] : 0;
        if (x > y) cmp = 1;
        else if (x < y) cmp = -1;
    }
    if (cmp < 0) return;
    for (i = 0; i < len; i++)
        for (j = 0; j < a003867_part[i]; j++)
            prod *= (unsigned long long)(a003867_part[i] - j + c[j] - i - 1);
    {
        unsigned long long f = 479001600ULL / prod;
        if (cmp == 0) {
            a003867_degs[a003867_cnt++] = f / 2;
            a003867_degs[a003867_cnt++] = f / 2;
        } else {
            a003867_degs[a003867_cnt++] = f;
        }
    }
}

static void a003867_gen(int rem, int maxp, int len) {
    int p;
    if (rem == 0) { a003867_process(len); return; }
    for (p = rem < maxp ? rem : maxp; p >= 1; p--) {
        a003867_part[len] = p;
        a003867_gen(rem - p, p, len + 1);
    }
}

static void a003867_init(void) {
    int i, j;
    a003867_cnt = 0;
    a003867_gen(12, 12, 0);
    for (i = 1; i < a003867_cnt; i++) {
        unsigned long long v = a003867_degs[i];
        j = i;
        while (j > 0 && a003867_degs[j - 1] > v) {
            a003867_degs[j] = a003867_degs[j - 1];
            j--;
        }
        a003867_degs[j] = v;
    }
}

seq_t a(idx_t n) {
    if (a003867_cnt < 0) a003867_init();
    if (n < 1 || n > (idx_t)a003867_cnt) return (seq_t)0;
    return (seq_t)a003867_degs[n - 1];
}
