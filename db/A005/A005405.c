/*
 * A005405  Number of protruded partitions of n with largest part at most 4.
 * https://oeis.org/A005405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: (1-x)^4/Product(1-x-x^i+x^(1+2*i), i=1..4) - 1. With P(x)
 *            = Product(1-x-x^i+x^(1+2*i), i=1..4) = sum p_j x^j and f(x) =
 *            (1-x)^4/P(x): f(i) = [x^i](1-x)^4 - sum_{j=1..24} p_j*f(i-j),
 *            f(0) = 1, and a(n) = f(n) for n >= 1
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static long long pc[25];
static int pc_ready = 0;

static void init_poly(void) {
    long long tmp[25];
    int i, j;
    if (pc_ready) return;
    memset(pc, 0, sizeof(pc));
    pc[0] = 1;
    for (i = 1; i <= 4; i++) {
        memset(tmp, 0, sizeof(tmp));
        for (j = 0; j <= 24; j++) {
            if (pc[j] == 0) continue;
            tmp[j] += pc[j];
            if (j + 1 <= 24) tmp[j + 1] -= pc[j];
            if (j + i <= 24) tmp[j + i] -= pc[j];
            if (j + 1 + 2 * i <= 24) tmp[j + 1 + 2 * i] += pc[j];
        }
        memcpy(pc, tmp, sizeof(pc));
    }
    pc_ready = 1;
}

seq_t a(idx_t n) {
    static const long long rhs[5] = {1, -4, 6, -4, 1};
    seq_t *f;
    seq_t res;
    idx_t i, j;
    if (n < 1) return 0;
    init_poly();
    f = alloc_zero(n + 1);
    for (i = 0; i <= n; i++) {
        seq_t pos = 0, neg = 0;
        long long r = (i < 5) ? rhs[i] : 0;
        if (r > 0) pos += (idx_t)r;
        else if (r < 0) neg += (idx_t)(-r);
        for (j = 1; j <= 24 && j <= i; j++) {
            long long c = pc[j];
            if (c > 0) neg += f[i - j] * (idx_t)c;
            else if (c < 0) pos += f[i - j] * (idx_t)(-c);
        }
        f[i] = pos - neg;
    }
    res = f[n];
    release(f, n + 1);
    return res;
}
