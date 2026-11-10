/*
 * A060167  Number of orbits of length n under the map whose periodic points
 *          are counted by A001642.
 * https://oeis.org/A060167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/n)*Sum_{d|n} mu(d)*A001642(n/d); A001642(n) =
 *            A001642(n-1)+A001642(n-2)+A001642(n-4)+A001642(n-5), with
 *            A001642(1..5) = 1, 3, 4, 11, 21
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* SHARED_HELPER: Moebius function mu(m) by trial division */
static int A060167_mu(idx_t m) {
    int r = 1;
    idx_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            r = -r;
        }
    }
    if (m > 1) r = -r;
    return r;
}

seq_t a(idx_t n) {
    seq_t *f;
    seq_t pos = 0, neg = 0, res;
    idx_t i, d;
    f = mem_new(seq_t, (size_t)n + 6);
    f[1] = 1; f[2] = 3; f[3] = 4; f[4] = 11; f[5] = 21;
    for (i = 6; i <= n; i++) {
        f[i] = f[i - 1] + f[i - 2] + f[i - 4] + f[i - 5];
    }
    for (d = 1; d <= n; d++) {
        if (n % d == 0) {
            int m = A060167_mu(d);
            if (m == 1) pos += f[n / d];
            else if (m == -1) neg += f[n / d];
        }
    }
    res = (pos - neg) / (seq_t)n;
    mem_free(f);
    return res;
}
