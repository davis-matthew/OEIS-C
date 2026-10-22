/*
 * A000939  Number of inequivalent n-gons.
 * https://oeis.org/A000939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = A002619(n)/2 for odd n >= 3, a(n) = (A002619(n) +
 *            A002866(n/2-1))/2 for even n >= 4, where A002619(n) = (1/n^2) *
 *            Sum_{d|n} phi(n/d)^2 * d! * (n/d)^d and A002866(k) = 2^(k-1)*k!;
 *            a(1) = a(2) = 1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   a939_mulk, a939_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a939_mulk.h"
#include "../utils/a939_phi.h"

seq_t a(idx_t n) {
    if (n <= 2) return 1;
    uint64_t N = (uint64_t)n;
    uint64_t D = 2 * N * N;
    seq_t Q = 0;
    uint64_t R = 0;
    for (uint64_t d = 1; d <= N; d++) {
        if (N % d != 0) continue;
        uint64_t m = N / d;
        uint64_t ph = a939_phi(m);
        seq_t q = 0;
        uint64_t r = ph * ph;
        for (uint64_t i = 2; i <= d; i++) a939_mulk(&q, &r, i, D);
        if (m > 1)
            for (uint64_t i = 0; i < d; i++) a939_mulk(&q, &r, m, D);
        Q += q;
        R += r;
    }
    if (N % 2 == 0) {
        uint64_t k = N / 2 - 1;
        seq_t q = 0;
        uint64_t r = N * N;
        for (uint64_t i = 1; i < k; i++) a939_mulk(&q, &r, 2, D);
        for (uint64_t i = 2; i <= k; i++) a939_mulk(&q, &r, i, D);
        Q += q;
        R += r;
    }
    return Q + (seq_t)(R / D);
}
