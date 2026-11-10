/*
 * A066980  Numbers k such that antid(k) > antid(k+1) and santid(k) >
 *          santid(k+1), where antid(k) = A066272 and santid(k) = A066417.
 * https://oeis.org/A066980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th k such that A066272(k) > A066272(k+1) and A066417(k)
 *            > A066417(k+1); found by incremental search with cached terms
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9869 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void A066980_divs(idx_t m, idx_t *cnt, idx_t *sig)
{
    idx_t c = 0, s = 0;
    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            idx_t e = m / d;
            c++; s += d;
            if (e != d) { c++; s += e; }
        }
    }
    *cnt = c; *sig = s;
}

static void A066980_anti(idx_t n, idx_t *ant, idx_t *san)
{
    if (n <= 1) { *ant = 0; *san = 0; return; }
    idx_t c1, s1, c3, s3, c2, s2;
    A066980_divs(2 * n - 1, &c1, &s1);
    A066980_divs(2 * n + 1, &c3, &s3);
    idx_t k = 0, o = n;
    while ((o & 1) == 0) { o >>= 1; ++k; }
    A066980_divs(o, &c2, &s2);
    *ant = c1 + c3 + c2 - 5;
    *san = s1 + s3 + s2 * ((idx_t)1 << (k + 1)) - 6 * n - 2;
}

static idx_t *A066980_terms = 0;
static size_t A066980_cnt = 0, A066980_cap = 0;
static idx_t A066980_k = 1;
static idx_t A066980_ca = 0, A066980_cs = 0;

seq_t a(idx_t n)
{
    while (A066980_cnt < (size_t)n) {
        idx_t na, ns;
        A066980_anti(A066980_k + 1, &na, &ns);
        if (A066980_ca > na && A066980_cs > ns) {
            if (A066980_cnt == A066980_cap) {
                A066980_cap = A066980_cap ? A066980_cap * 2 : 1024;
                A066980_terms = (idx_t *)realloc(A066980_terms, A066980_cap * sizeof(idx_t));
            }
            A066980_terms[A066980_cnt++] = A066980_k;
        }
        A066980_ca = na; A066980_cs = ns;
        A066980_k++;
    }
    return (seq_t)A066980_terms[n - 1];
}
