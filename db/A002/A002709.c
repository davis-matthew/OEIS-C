/*
 * A002709  Triangulations of the disk G_{n,0}.
 * https://oeis.org/A002709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A000260(n) + 2*F(n))/3, where A000260(n) =
 *            6*(4n+1)!/(n!*(3n+3)!) counts rooted triangulations (Brown), and
 *            F(n) counts the triangulations with 3-fold rotational symmetry:
 *            F(3k) = binomial(4k,k)/(3k+1), F(3k+1) =
 *            binomial(4k+2,k)/(2k+1), F(3k+2) = 0. (Burnside's lemma under
 *            rotation of th...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 24, u128 n <= 44,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 499 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static void a2709_add_fact(long *e, const unsigned char *comp, idx_t x, long s)
{
    for (idx_t p = 2; p <= x; p++) {
        if (comp[p]) continue;
        idx_t q = x;
        long c = 0;
        while (q) { q /= p; c += (long)q; }
        e[p] += s * c;
    }
}

static void a2709_add_int(long *e, idx_t x, long s)
{
    for (idx_t p = 2; p * p <= x; p++)
        while (x % p == 0) { e[p] += s; x /= p; }
    if (x > 1) e[x] += s;
}

/* Product P of p^e[p]: returns Q = floor(P/3), *r = P mod 3 (no overflow beyond P/3). */
static seq_t a2709_div3(const long *e, idx_t N, uint64_t *r)
{
    seq_t q = 0;
    uint64_t rr = 1, m = 1;
    for (idx_t p = 2; p <= N; p++) {
        for (long j = 0; j < e[p]; j++) {
            if (m > 0xFFFFFFFFull / p) {
                q = q * m + (rr * m) / 3;
                rr = (rr * m) % 3;
                m = 1;
            }
            m *= p;
        }
    }
    q = q * m + (rr * m) / 3;
    rr = (rr * m) % 3;
    *r = rr;
    return q;
}

seq_t a(idx_t n)
{
    idx_t N = 4 * n + 4;
    unsigned char *comp = (unsigned char *)calloc(N + 1, 1);
    long *e = (long *)calloc(N + 1, sizeof(long));
    for (idx_t i = 2; i * i <= N; i++)
        if (!comp[i])
            for (idx_t j = i * i; j <= N; j += i) comp[j] = 1;

    /* R = 6*(4n+1)!/(n!(3n+3)!) */
    a2709_add_int(e, 6, 1);
    a2709_add_fact(e, comp, 4 * n + 1, 1);
    a2709_add_fact(e, comp, n, -1);
    a2709_add_fact(e, comp, 3 * n + 3, -1);
    uint64_t rR = 0, rF = 0;
    seq_t result = a2709_div3(e, N, &rR);

    if (n % 3 != 2) {
        for (idx_t i = 0; i <= N; i++) e[i] = 0;
        idx_t k = n / 3;
        a2709_add_int(e, 2, 1);
        if (n % 3 == 0) {
            a2709_add_fact(e, comp, 4 * k, 1);
            a2709_add_fact(e, comp, k, -1);
            a2709_add_fact(e, comp, 3 * k + 1, -1);
        } else {
            a2709_add_fact(e, comp, 4 * k + 2, 1);
            a2709_add_fact(e, comp, k, -1);
            a2709_add_fact(e, comp, 3 * k + 2, -1);
            a2709_add_int(e, 2 * k + 1, -1);
        }
        result += a2709_div3(e, N, &rF);
    }
    result += (rR + rF) / 3;

    free(comp);
    free(e);
    return result;
}
