/*
 * A003001  Smallest number of multiplicative persistence n.
 * https://oeis.org/A003001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, base, more, hard
 * Formula:   search. For n >= 2 the terms use only digits 2..9, in
 *            nondecreasing order (Corneth). So for each length L = 2, 3, ...
 *            we list the nondecreasing digit strings in increasing order.
 *            a(n) is the first one whose digit product has persistence n-1.
 *            For n < 2 a direct scan gives a(0) = 0, a(1) = 10
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int pers_u64(uint64_t x) {
    int c = 0;
    while (x >= 10) {
        uint64_t p = 1;
        while (x) { p *= x % 10; x /= 10; }
        x = p;
        c++;
    }
    return c;
}

static int dfs_a003001(int pos, int L, int mind, uint64_t prod, int target, int *dig) {
    int d;
    if (pos == L) return pers_u64(prod) == target;
    for (d = mind; d <= 9; d++) {
        dig[pos] = d;
        if (dfs_a003001(pos + 1, L, d, prod * (uint64_t)d, target, dig)) return 1;
    }
    return 0;
}

seq_t a(idx_t n) {
    static seq_t cache[32];
    static int done[32] = {0};
    int dig[24];
    int L, i;
    seq_t v;

    if (n < 2) {
        uint64_t m = 0;
        while (pers_u64(m) != (int)n) m++;
        return (seq_t)m;
    }
    if (n >= 32) return 0;
    if (done[n]) return cache[n];

    v = 0;
    /* 9^20 < 2^64, so the digit product always fits in uint64_t */
    for (L = 2; L <= 20; L++) {
        if (dfs_a003001(0, L, 2, 1, (int)n - 1, dig)) {
            v = 0;
            for (i = 0; i < L; i++) v = v * 10 + dig[i];
            break;
        }
    }
    cache[n] = v;
    done[n] = 1;
    return v;
}
