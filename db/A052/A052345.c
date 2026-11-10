/*
 * A052345  Least k such that A052343(k)=n.
 * https://oeis.org/A052345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with ceil(R(4k+1)/2) = n, where R(m) = prod (e+1)
 *            over primes p == 1 (mod 4) dividing m (R = 0 if a prime == 3
 *            (mod 4) divides m to an odd power), since T_i + T_j = k <=>
 *            (i+j+1)^2 + (i-j)^2 = 4k+1. Searched by enumerating exponent
 *            patterns on the primes 5, 13, 17, 29, ... (a(0) =...
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A052345_NP 64
static unsigned A052345_pr[A052345_NP];
static int A052345_init = 0;

static void A052345_build(void)
{
    int c = 0;
    unsigned p;
    for (p = 5; c < A052345_NP; p += 4) {
        unsigned q;
        int isp = 1;
        for (q = 3; q * q <= p; q += 2)
            if (p % q == 0) { isp = 0; break; }
        if (isp) A052345_pr[c++] = p;
    }
    A052345_init = 1;
}

typedef struct {
    seq_t best;
    int have;
} A052345_state;

/* k -> k' with 4k'+1 = (4k+1)*p ; returns 0 on overflow */
static int A052345_step(seq_t k, unsigned p, seq_t *out)
{
    seq_t sp = (seq_t)p;
    if (sp / 4 != (seq_t)(p / 4)) return 0;
    seq_t t = k * sp;
    if (k != 0 && t / sp != k) return 0;
    seq_t u = t + (seq_t)((p - 1) / 4);
    if (u < t) return 0;
    *out = u;
    return 1;
}

static void A052345_dfs(A052345_state *st, size_t j, idx_t r, idx_t maxf, seq_t k)
{
    if (r == 1) {
        if (!st->have || k < st->best) {
            st->best = k;
            st->have = 1;
        }
        return;
    }
    if (j >= A052345_NP) return;
    idx_t top = r < maxf ? r : maxf;
    for (idx_t f = 2; f <= top; f++) {
        if (r % f != 0) continue;
        seq_t cur = k;
        int ok = 1;
        for (idx_t e = 0; e + 1 < f; e++) {
            seq_t nx;
            if (!A052345_step(cur, A052345_pr[j], &nx)) { ok = 0; break; }
            cur = nx;
            if (st->have && cur >= st->best) { ok = 0; break; }
        }
        if (!ok) continue;
        A052345_dfs(st, j + 1, r / f, f, cur);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)1;
    if (!A052345_init) A052345_build();
    A052345_state st;
    st.best = 0;
    st.have = 0;
    A052345_dfs(&st, 0, (idx_t)2 * n, (idx_t)2 * n, (seq_t)0);
    A052345_dfs(&st, 0, (idx_t)2 * n - 1, (idx_t)2 * n - 1, (seq_t)0);
    return st.best;
}
