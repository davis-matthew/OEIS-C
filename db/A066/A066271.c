/*
 * A066271  Numbers n such that A066256(n) = n^2-2*n+2.
 * https://oeis.org/A066271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m>=3 such that A066256(m) = m^2-2m+2;
 *            computed by search: with d = m^2+k, k=m^2-2m+2 gives d0 =
 *            2m^2-2m+2, which must divide m^4+m, and no divisor of m^4+m may
 *            lie in (m^2+m, d0)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A066271_MAXF 80

typedef struct {
    seq_t p[A066271_MAXF];
    int e[A066271_MAXF];
    int cnt;
} A066271_fac;

static void A066271_addp(A066271_fac *f, seq_t p, int e)
{
    for (int i = 0; i < f->cnt; i++) {
        if (f->p[i] == p) { f->e[i] += e; return; }
    }
    f->p[f->cnt] = p;
    f->e[f->cnt] = e;
    f->cnt++;
}

static void A066271_factor(A066271_fac *f, seq_t x)
{
    if (x < 2) return;
    int e = 0;
    while (x % 2 == 0) { x /= 2; e++; }
    if (e) A066271_addp(f, (seq_t)2, e);
    for (seq_t p = 3; p * p <= x; p += 2) {
        e = 0;
        while (x % p == 0) { x /= p; e++; }
        if (e) A066271_addp(f, p, e);
    }
    if (x > 1) A066271_addp(f, x, 1);
}

/* is there a divisor in the open range (lo, hi)? */
static bool A066271_dfs(const A066271_fac *f, int i, seq_t cur, seq_t lo, seq_t hi)
{
    if (cur >= hi) return false;
    if (cur > lo) return true;
    if (i >= f->cnt) return false;
    seq_t c = cur;
    for (int j = 0; j <= f->e[i]; j++) {
        if (A066271_dfs(f, i + 1, c, lo, hi)) return true;
        if (j < f->e[i]) {
            c *= f->p[i];
            if (c >= hi) break;
        }
    }
    return false;
}

static bool A066271_is_term(idx_t m)
{
    seq_t s = (seq_t)m;
    seq_t d0 = 2 * s * s - 2 * s + 2;
    seq_t N = s * s * s * s + s;
    if (N % d0 != 0) return false;
    seq_t lo = s * s + s;
    A066271_fac f;
    f.cnt = 0;
    A066271_factor(&f, s);
    A066271_factor(&f, s + 1);
    A066271_factor(&f, s * s - s + 1);
    return !A066271_dfs(&f, 0, (seq_t)1, lo, d0);
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 3; ; ++m) {
        if (A066271_is_term(m)) {
            ++count;
            if (count == n) return (seq_t)m;
        }
    }
}
