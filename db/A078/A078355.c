/*
 * A078355  Minimal (positive) solution a(n) of Pell equation b(n)^2 -
 *          D(n)*a(n)^2 = +4 with D(n)= A077425(n). The companion sequence is
 *          a(n)=A077428(n).
 * https://oeis.org/A078355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal positive a with b^2 - D*a^2 = 4, D = A077425(n);
 *            found by the continued fraction of (1+sqrt(D))/2 (search)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 41, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A078355_isqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (hi * hi <= x) hi <<= 1;
    while (lo + 1 < hi) {
        seq_t mid = (lo + hi) >> 1;
        if (mid * mid <= x) lo = mid; else hi = mid;
    }
    return lo;
}

static seq_t A078355_D(idx_t n)
{
    idx_t count = 0;
    seq_t cand = 1;
    while (1) {
        cand += 4;
        seq_t r = A078355_isqrt(cand);
        if (r * r != cand) {
            ++count;
            if (count == n) return cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t D = A078355_D(n);
    seq_t s = A078355_isqrt(D);
    seq_t P = 1, Q = 2;
    seq_t kprev = 0, kcur = 0;
    seq_t kk = 0;
    bool first = true;
    idx_t idx = 0;
    while (1) {
        seq_t ak = (P + s) / Q;
        if (first) {
            kprev = 0; kcur = 1;
            first = false;
        } else {
            seq_t knew = ak * kcur + kprev;
            kprev = kcur; kcur = knew;
        }
        seq_t Pn = ak * Q - P;
        seq_t Qn = (D - Pn * Pn) / Q;
        kk = kcur;
        if (Qn == 2 && (idx & 1)) return kk;
        P = Pn; Q = Qn;
        ++idx;
    }
}
