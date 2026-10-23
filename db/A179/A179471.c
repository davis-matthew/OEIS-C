/*
 * A179471  G.f. satisfies: A(x) = exp( Sum_{n>=1} A(2^n*x^n)*x^n/n ).
 * https://oeis.org/A179471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A179471
    G.f.: A(x) = exp( Σ_{n>=1} A(2^n*x^n)*x^n/n )
    Recurrence (derived from the functional equation):
        a(0) = 1
        let s(n) = n * Σ_{d|n} a(n/d-1) * 2^{d*(n/d-1)} / d   (i.e. s(n)=n*l(n))
        for n ≥ 1:
            a(n) = (1/n) * Σ_{k=1}^{n} s(k) * a(n-k)              */

/*  This implementation caches the values of a and s in dynamically
    resizable arrays so that calls can be made in any order.  It is
    valid for both C11 and C++17.  When compiled as C++, std::vector
    is used; otherwise malloc/realloc are employed.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp) */

#ifdef __cplusplus
#include <vector>
#endif

/* Cached tables.  */
static idx_t built = 0;          /* index of the largest computed coefficient */
static bool init = false;        /* have a(0) been initialized? */
#ifdef __cplusplus
static std::vector<seq_t> a_tbl;
static std::vector<seq_t> s_tbl;
#else
static seq_t *a_tbl = NULL;
static seq_t *s_tbl = NULL;
static size_t cap = 0;           /* allocated size of the tables */
#endif

/* Ensure that the tables are sized to hold indices up to N (inclusive).  */
static void
ensure_capacity(idx_t N)
{
#ifdef __cplusplus
    if ((idx_t)a_tbl.size() > N) return;
    a_tbl.resize(N + 1);
    s_tbl.resize(N + 1);
#else
    if (N < (idx_t)cap) return;
    size_t new_cap = cap ? cap << 1 : 1;
    while (new_cap <= (size_t)N) new_cap <<= 1;
    a_tbl = (seq_t *)realloc(a_tbl, new_cap * sizeof(seq_t));
    s_tbl = (seq_t *)realloc(s_tbl, new_cap * sizeof(seq_t));
    /* newly allocated elements are zero-initialised (malloc may not zero,
       but we set them explicitly). */
    for (size_t i = cap; i < new_cap; ++i) {
        a_tbl[i] = 0;
        s_tbl[i] = 0;
    }
    cap = new_cap;
#endif
}

/* Return the nth term of A179471.  */
seq_t
a(idx_t n)
{
    /* Initialise a(0) once.  */
    if (!init) {
        ensure_capacity(0);
        a_tbl[0] = 1;          /* a(0) = 1 */
        s_tbl[0] = 0;          /* not used but set for consistency */
        init = true;
        built = 0;
    }

    if (n == 0) return a_tbl[0];

    ensure_capacity(n);

    /* Compute missing terms up to n.  */
    for (idx_t m = built + 1; m <= n; ++m) {
        /* ---- compute s(m) ---- */
        seq_t sm = 0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d != 0) continue;

            /* divisor d */
            idx_t mdiv = m / d;            /* m/d */
            idx_t k    = mdiv - 1;         /* index for a */

            seq_t term = a_tbl[k];
            /* multiply by 2^{d*(m/d-1)} */
            term *= pow2(d * k);
            /* multiply by (m/d) */
            term *= (seq_t)mdiv;
            sm += term;

            if (d != mdiv) { /* paired divisor */
                idx_t d2 = mdiv;
                idx_t mdiv2 = m / d2;     /* = d */
                idx_t k2 = mdiv2 - 1;

                seq_t term2 = a_tbl[k2];
                term2 *= pow2(d2 * k2);
                term2 *= (seq_t)mdiv2;
                sm += term2;
            }
        }
        s_tbl[m] = sm;

        /* ---- compute a(m) ---- */
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            sum += s_tbl[k] * a_tbl[m - k];
        }
        a_tbl[m] = sum / (seq_t)m;          /* exact division */
    }

    built = n;
    return a_tbl[n];
}
