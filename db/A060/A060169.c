/*
 * A060169  Number of orbits of length n under the automorphism of the 3-torus
 *          whose periodic points are counted by A001945.
 * https://oeis.org/A060169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/n)*Sum_{d|n} mu(d)*A001945(n/d), where A001945(n+6) =
 *            -A001945(n+5) + A001945(n+4) + 3*A001945(n+3) + A001945(n+2) -
 *            A001945(n+1) - A001945(n), A001945(-2) = A001945(-1) = -1,
 *            A001945(0) = 0, A001945(1) = A001945(2) = A001945(3) = 1
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A001945_a(idx_t n) {
    if (n == 0) return 0;
    if (n <= 3) return 1;
    seq_t p6 = (seq_t)(-1);
    seq_t p5 = (seq_t)(-1);
    seq_t p4 = 0;
    seq_t p3 = 1;
    seq_t p2 = 1;
    seq_t p1 = 1;
    seq_t result = 0;
    for (idx_t i = 4; i <= n; i++) {
        result = -p1 + p2 + 3 * p3 + p4 - p5 - p6;
        p6 = p5; p5 = p4; p4 = p3; p3 = p2; p2 = p1; p1 = result;
    }
    return result;
}

static int A060169_mu(idx_t m) {
    int r = 1;
    for (idx_t p = 2; p * p <= m; p++) {
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
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; d++) {
        if (n % d != 0) continue;
        int mu = A060169_mu(d);
        if (mu == 0) continue;
        seq_t v = A001945_a(n / d);
        if (mu > 0) sum += v;
        else sum -= v;
    }
    return sum / (seq_t)n;
}
