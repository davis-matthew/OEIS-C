/*
 * A000473  Number of genus 0 rooted maps with 5 faces and n vertices.
 * https://oeis.org/A000473
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.
 *            x^3*(1-sqrt(1-4*x))*(17+16*x-(10+4*x)*sqrt(1-4*x))/(1-4*x)^(11/2)
 *            simplifies to x^3*((27-20*x-16*x^2)*(1-4*x)^(-11/2) -
 *            (27+20*x)*(1-4*x)^(-5)). With m = n-3, q(k) =
 *            [x^k](1-4*x)^(-11/2), q(0) = 1, q(k) = q(k-1)*(4k+18)/k, and
 *            p(k) = [x^k](1-4*x)^(-5) = binomial(k+4,4)*4^k: a(n) = 27*q(m)
 *            -...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 55,
 *            big any n
 * Helpers:   a473_g, a473_p
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a473_g.h"
#include "../utils/a473_p.h"

seq_t a(idx_t n) {
    if (n < 4) return 0;
    idx_t m = n - 3;
    seq_t q0 = 0, q1 = 0, q2 = 1; /* q(k-2), q(k-1), q(k) */
    seq_t pw = 1, pwprev = 1;
    for (idx_t k = 1; k <= m; k++) {
        uint64_t num = 4 * (uint64_t)k + 18;
        uint64_t g = a473_g(num, (uint64_t)k);
        seq_t nq = (q2 / (seq_t)(k / g)) * (seq_t)(num / g);
        q0 = q1; q1 = q2; q2 = nq;
        pwprev = pw;
        pw *= 4;
    }
    seq_t pos = 27 * q2;
    seq_t neg = 20 * q1 + 16 * q0 + 27 * a473_p(m, pw) + 20 * a473_p(m - 1, pwprev);
    return pos - neg;
}
