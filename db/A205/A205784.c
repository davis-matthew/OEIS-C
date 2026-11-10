/*
 * A205784  Least positive integer j such that n divides C(k)-C(j), where k,
 *          as in A205782, is the least number for which there is such a j,
 *          and C=A205824.
 * https://oeis.org/A205784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least j with C(j) ≡ C(k) (mod n), k = A205782(n) the
 *            first index whose C-value collides mod n with an earlier one;
 *            C(i)=A205824(i)=(3i)!/(3i*i!*(i+1)!). Computed by search over
 *            prime-exponent vectors
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Validated: partial, pass, 96 terms through n = 96 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void A205784_upd(int *e, uint64_t m, int s) {
    for (uint64_t d = 2; d * d <= m; ++d) {
        while (m % d == 0) { e[d] += s; m /= d; }
    }
    if (m > 1) e[m] += s;
}

static uint64_t A205784_pw(uint64_t b, int ex, uint64_t n) {
    unsigned __int128 r = 1 % n, x = b % n;
    while (ex > 0) {
        if (ex & 1) r = (r * x) % n;
        x = (x * x) % n;
        ex >>= 1;
    }
    return (uint64_t)r;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    uint64_t N = (uint64_t)n;
    size_t sz = (size_t)(3 * (N + 2) + 4);
    int *e = (int *)calloc(sz, sizeof(int));
    uint64_t *first = (uint64_t *)calloc((size_t)N, sizeof(uint64_t));
    if (!e || !first) abort();
    uint64_t ans = 1;
    for (uint64_t i = 1; i <= N + 1; ++i) {
        if (i >= 2) {
            A205784_upd(e, 3 * i - 1, 1);
            A205784_upd(e, 3 * i - 2, 1);
            A205784_upd(e, 3 * i - 3, 1);
            A205784_upd(e, i, -1);
            A205784_upd(e, i + 1, -1);
        }
        unsigned __int128 r = 1 % N;
        for (uint64_t p = 2; p <= 3 * i; ++p) {
            if (e[p] > 0) r = (r * A205784_pw(p, e[p], N)) % N;
        }
        uint64_t res = (uint64_t)r;
        if (first[res]) { ans = first[res]; break; }
        first[res] = i;
    }
    free(e);
    free(first);
    return (seq_t)ans;
}
