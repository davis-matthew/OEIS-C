/*
 * A060166  Number of orbits of length n under the map whose periodic points
 *          are counted by A001641.
 * https://oeis.org/A060166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/n)*Sum_{d|n} mu(d)*A001641(n/d), where A001641(m) =
 *            A001641(m-1) + A001641(m-2) + A001641(m-4), A001641(1)=1,
 *            A001641(2)=3, A001641(3)=4, A001641(4)=11. Each A001641(m) is
 *            tracked as quotient and remainder modulo n, so that the final
 *            division is exact even when A001641(n) itself does no...
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* SHARED_HELPER: Moebius function mu(m) as int, by trial division */
static int A060166_mu(idx_t m) {
    int res = 1;
    idx_t p;
    if (m == 1) return 1;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            res = -res;
        }
    }
    if (m > 1) res = -res;
    return res;
}

seq_t a(idx_t n) {
    size_t sz = (size_t)n + 5;
    seq_t *q = mem_new(seq_t, sz);
    uint64_t *r = (uint64_t *)malloc(sz * sizeof(uint64_t));
    uint64_t init[5] = {0, 1, 3, 4, 11};
    idx_t i, d;
    seq_t posQ = 0, negQ = 0, res;
    long long rdiff = 0;

    for (i = 1; i <= 4; i++) {
        q[i] = (seq_t)(init[i] / n);
        r[i] = init[i] % n;
    }
    for (i = 5; i <= n; i++) {
        uint64_t s = r[i - 1] + r[i - 2] + r[i - 4];
        q[i] = q[i - 1] + q[i - 2] + q[i - 4] + (seq_t)(s / n);
        r[i] = s % n;
    }

    for (d = 1; d <= n; d++) {
        int mu;
        idx_t m;
        if (n % d != 0) continue;
        mu = A060166_mu(d);
        if (mu == 0) continue;
        m = n / d;
        if (mu == 1) {
            posQ += q[m];
            rdiff += (long long)r[m];
        } else {
            negQ += q[m];
            rdiff -= (long long)r[m];
        }
    }

    if (rdiff >= 0) {
        res = posQ + (seq_t)(uint64_t)(rdiff / (long long)n);
        res -= negQ;
    } else {
        res = posQ - negQ;
        res -= (seq_t)(uint64_t)((-rdiff) / (long long)n);
    }

    mem_free(q);
    free(r);
    return res;
}
