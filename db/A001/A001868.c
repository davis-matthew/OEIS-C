/*
 * A001868  Number of n-bead necklaces with 4 colors.
 * https://oeis.org/A001868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (1/n)*Sum_{d|n} phi(d)*4^(n/d), a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 197 (2026-11-01)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

/* phi * 4^e = (*q) * n + (*r), with 0 <= *r < n */
static void a001868_term(uint64_t ph, idx_t e, idx_t n, seq_t *q, uint64_t *r) {
    seq_t Q = (seq_t)(uint64_t)(1 / n);
    uint64_t R = 1 % n;
    idx_t i;
    for (i = 0; i < e; i++) {
        uint64_t t = R << 2;
        Q = (Q << 2) + (seq_t)(t / n);
        R = t % n;
    }
    {
        unsigned __int128 t = (unsigned __int128)R * ph;
        Q = Q * (seq_t)ph + (seq_t)(uint64_t)(t / n);
        R = (uint64_t)(t % n);
    }
    *q = Q;
    *r = R;
}

seq_t a(idx_t n) {
    seq_t sumq = 0;
    unsigned __int128 sumr = 0;
    idx_t d;
    if (n == 0) return 1;
    for (d = 1; d <= n; d++) {
        if (n % d == 0) {
            seq_t q;
            uint64_t r;
            a001868_term(totient_u64(d), n / d, n, &q, &r);
            sumq += q;
            sumr += r;
        }
    }
    sumq += (seq_t)(uint64_t)(sumr / n);
    return sumq;
}
