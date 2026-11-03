/*
 * A003639  Number of classes per genus in Q(sqrt -n), n squarefree.
 * https://oeis.org/A003639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(D) / 2^(omega(|D|)-1), where m = A005117(n) (the n-th
 *            squarefree number), D = -m if m == 3 (mod 4) else D = -4m, h(D)
 *            = class number (counted as reduced binary quadratic forms),
 *            omega = number of distinct prime factors
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 104 terms through n = 104 (2026-11-01)
 * Helpers:   is_squarefree_i64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_squarefree_i64.h"

static long long nth_squarefree(idx_t n) {
    static long long *cache = 0;
    static idx_t cnt = 0, cap = 0;
    static long long last = 0;
    if (n <= cnt) return cache[n - 1];
    while (cnt < n) {
        last++;
        if (is_squarefree_i64(last)) {
            if (cnt == cap) {
                idx_t nc = cap ? cap * 2 : 1024;
                long long *nb = (long long *)malloc((size_t)nc * sizeof(long long));
                idx_t i;
                for (i = 0; i < cnt; i++) nb[i] = cache[i];
                free(cache);
                cache = nb;
                cap = nc;
            }
            cache[cnt++] = last;
        }
    }
    return cache[n - 1];
}

static int omega_ll(long long x) {
    int t = 0;
    long long p;
    for (p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            t++;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) t++;
    return t;
}

seq_t a(idx_t n) {
    long long m = nth_squarefree(n);
    long long absD = (m % 4 == 3) ? m : 4 * m;
    long long h = 0, A, b;
    int t;
    for (A = 1; 3 * A * A <= absD; A++) {
        for (b = -A + 1; b <= A; b++) {
            long long num = b * b + absD;
            long long c;
            if (num % (4 * A) != 0) continue;
            c = num / (4 * A);
            if (c < A) continue;
            if (c == A && b < 0) continue;
            h++;
        }
    }
    t = omega_ll(absD);
    return (seq_t)(h >> (t - 1));
}
