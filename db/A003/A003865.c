/*
 * A003865  Degrees of irreducible representations of alternating group A_10.
 * https://oeis.org/A003865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   degrees of the irreducible characters of A_10 come from the hook
 *            length formula f(L) = 10!/prod(hooks) applied to partitions L of
 *            10: each pair {L, L'} with L != L' gives one character of degree
 *            f(L), and each self-conjugate L gives two characters of degree
 *            f(L)/2; a(n) is the n-th smallest of th...
 * Range:     u8 n <= 15, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   an_cmp_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/an_cmp_u64.h"

#define AN_N 10

static uint64_t an_degs[64];
static int an_ndeg = 0;
static int an_done = 0;
static int an_part[AN_N + 1];

static void an_process(int len) {
    int lam[AN_N + 1], conj[AN_N + 1];
    int i, j, cmp = 0;
    uint64_t fact = 1, prod = 1, f;
    for (i = 0; i <= AN_N; i++) lam[i] = (i < len) ? an_part[i] : 0;
    for (j = 0; j <= AN_N; j++) {
        int c = 0;
        for (i = 0; i < len; i++) if (lam[i] > j) c++;
        conj[j] = c;
    }
    for (i = 0; i <= AN_N; i++) {
        if (lam[i] != conj[i]) { cmp = (lam[i] > conj[i]) ? 1 : -1; break; }
    }
    for (i = 0; i < len; i++)
        for (j = 0; j < lam[i]; j++)
            prod *= (uint64_t)((lam[i] - j - 1) + (conj[j] - i - 1) + 1);
    for (i = 2; i <= AN_N; i++) fact *= (uint64_t)i;
    f = fact / prod;
    if (cmp == 0) {
        an_degs[an_ndeg++] = f / 2;
        an_degs[an_ndeg++] = f / 2;
    } else if (cmp > 0) {
        an_degs[an_ndeg++] = f;
    }
}

static void an_gen(int rem, int maxp, int len) {
    int p;
    if (rem == 0) { an_process(len); return; }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        an_part[len] = p;
        an_gen(rem - p, p, len + 1);
    }
}

seq_t a(idx_t n) {
    if (!an_done) {
        an_ndeg = 0;
        an_gen(AN_N, AN_N, 0);
        qsort(an_degs, (size_t)an_ndeg, sizeof(uint64_t), an_cmp_u64);
        an_done = 1;
    }
    if (n < 1 || n > (idx_t)an_ndeg) return (seq_t)0;
    return (seq_t)an_degs[(int)(n - 1)];
}
