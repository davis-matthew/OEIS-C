/*
 * A006593  Least number which is side of n Pythagorean triples.
 * https://oeis.org/A006593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A046081(x) = (L + H - 2)/2 with x = 2^e0 * prod p^f (p==1 mod 4)
 *            * prod q^g (q==3 mod 4), L = (e0>0 ? 2e0-1 : 1) * prod(2f+1) *
 *            prod(2g+1), H = prod(2f+1). So a(n) = min x with B*(A*C+1) =
 *            2n+2; search over divisor splits, taking the smallest number
 *            with each prescribed prod(2e+1) (exponents noni...
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 35, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

static long long pr1[24], pr3[24];
static int primes_ready = 0;

static void init_primes(void) {
    int c1 = 0, c3 = 0;
    long long p;
    if (primes_ready) return;
    for (p = 3; c1 < 24 || c3 < 24; p += 2) {
        long long d;
        bool ip = true;
        for (d = 3; d * d <= p; d += 2)
            if (p % d == 0) { ip = false; break; }
        if (!ip) continue;
        if (p % 4 == 1) { if (c1 < 24) pr1[c1++] = p; }
        else { if (c3 < 24) pr3[c3++] = p; }
    }
    primes_ready = 1;
}

static bool mul_ok(seq_t x, seq_t y, seq_t *r) {
    if (seq_is_fixed_width()) {
        seq_t mx = (seq_t)((seq_t)0 - 1);
        if (y != 0 && x > mx / y) return false;
    }
    *r = x * y;
    return true;
}

static bool pow_ok(long long p, long long e, seq_t *r) {
    seq_t v = 1;
    long long i;
    for (i = 0; i < e; i++)
        if (!mul_ok(v, (seq_t)p, &v)) return false;
    *r = v;
    return true;
}

static void rec(long long T, const long long *pr, int idx, long long maxf,
                seq_t cur, seq_t *best, bool *have) {
    long long f;
    if (T == 1) {
        if (!*have || cur < *best) { *best = cur; *have = true; }
        return;
    }
    if (idx >= 24) return;
    for (f = 3; f <= maxf && f <= T; f += 2) {
        seq_t pw, v;
        if (T % f != 0) continue;
        if (!pow_ok(pr[idx], (f - 1) / 2, &pw)) continue;
        if (!mul_ok(cur, pw, &v)) continue;
        if (*have && v >= *best) continue;
        rec(T / f, pr, idx + 1, f, v, best, have);
    }
}

static bool minprod(long long T, const long long *pr, seq_t *r) {
    seq_t best = 0;
    bool have = false;
    rec(T, pr, 0, T, (seq_t)1, &best, &have);
    if (have) *r = best;
    return have;
}

seq_t a(idx_t n) {
    long long m = 2 * (long long)n + 2;
    long long B, A;
    seq_t best = 0;
    bool have = false;
    init_primes();
    for (B = 1; B <= m; B += 2) {
        long long D;
        seq_t vb;
        if (m % B != 0) continue;
        D = m / B - 1;
        if (D < 1) continue;
        if (!minprod(B, pr1, &vb)) continue;
        for (A = 1; A <= D; A += 2) {
            seq_t vc, v2, v;
            if (D % A != 0) continue;
            if (!minprod(D / A, pr3, &vc)) continue;
            if (A == 1) v2 = 1;
            else if (!pow_ok(2, (A + 1) / 2, &v2)) continue;
            if (!mul_ok(vb, vc, &v)) continue;
            if (!mul_ok(v, v2, &v)) continue;
            if (!have || v < best) { best = v; have = true; }
        }
    }
    return best;
}
