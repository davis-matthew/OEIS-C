/*
 * A064528  Positive numbers that do not appear in A064520.
 * https://oeis.org/A064528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer not of the form A064520(k) =
 *            Sum_{j=1..k} (-1)^(A002024(j)+1)*j; computed by enumerating the
 *            values of A064520 in [1,L] run by run and sieving
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

typedef __int128 i128_t;

static i128_t A064528_f(i128_t k, i128_t t) {
    return k * t + k * (k + 1) / 2;
}

static void A064528_build(long long L, long long **out, size_t *cnt) {
    unsigned char *mark = (unsigned char *)malloc((size_t)L + 2);
    for (long long i = 0; i <= L + 1; i++) mark[i] = 0;
    i128_t S = 0;
    const long long M = 100000;
    for (long long m = 1; m <= M; m++) {
        i128_t t = (i128_t)(m - 1) * m / 2;
        bool plus = (m & 1) != 0;
        i128_t lo, hi;
        if (plus) { lo = 1 - S; hi = (i128_t)L - S; }
        else      { lo = S - (i128_t)L; hi = S - 1; }
        i128_t fm = A064528_f(m, t);
        if (!(hi < A064528_f(1, t) || lo > fm)) {
            long long a = 1, b = m;
            while (a < b) {
                long long mid = a + (b - a) / 2;
                if (A064528_f(mid, t) >= lo) b = mid; else a = mid + 1;
            }
            for (long long k = a; k <= m; k++) {
                i128_t f = A064528_f(k, t);
                if (f > hi) break;
                if (f < lo) continue;
                i128_t v = plus ? S + f : S - f;
                if (v >= 1 && v <= L) mark[(long long)v] = 1;
            }
        }
        S = plus ? S + fm : S - fm;
    }
    long long *res = (long long *)malloc(sizeof(long long) * ((size_t)L + 1));
    size_t c = 0;
    for (long long v = 1; v <= L; v++)
        if (!mark[v]) res[c++] = v;
    free(mark);
    *out = res;
    *cnt = c;
}

seq_t a(idx_t n) {
    static long long *list = NULL;
    static size_t count = 0;
    if (n == 0) return 0;
    if (list == NULL || count < (size_t)n) {
        long long L = 4 * (long long)n + 200;
        for (;;) {
            if (list) { free(list); list = NULL; count = 0; }
            A064528_build(L, &list, &count);
            if (count >= (size_t)n) break;
            L *= 2;
        }
    }
    return (seq_t)list[n - 1];
}
