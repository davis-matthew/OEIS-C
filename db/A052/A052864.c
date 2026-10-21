/*
 * A052864  E.g.f.: (log(1-x))^2/(1+log(1-x)).
 * https://oeis.org/A052864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, * a(1) = 0, * for n ≥ 2 * a(n) = 2 * Σ_{k=1}^{n-1}
 *            (n-1)! / k * + Σ_{i=2}^{n-1} a(i) * C(n,i) * (n-i-1)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implements the E.g.f.  (log(1-x))^2/(1+log(1-x)). */

#include <stddef.h>   /* for size_t */

/* maximum n for which the function will be called.
 * The problem statement guarantees n ≤ 10000.               */
#define MAX_N 10000

static seq_t a_seq[MAX_N + 1];
static seq_t fact_seq[MAX_N + 1];
static idx_t max_computed = 0;
static int initialized = 0;

seq_t a(idx_t n)
{
    /* initialise once */
    if (!initialized) {
        a_seq[0] = (seq_t)0;
        a_seq[1] = (seq_t)0;
        fact_seq[0] = (seq_t)1;
        fact_seq[1] = (seq_t)1;
        max_computed = 1;
        initialized = 1;
    }

    /* guard against out‑of‑bounds requests */
    if (n > (idx_t)MAX_N) {
        /* For safety: return 0 if n is beyond supported range. */
        return (seq_t)0;
    }

    /* compute values up to n if necessary */
    if (n > max_computed) {
        for (idx_t m = max_computed + 1; m <= n; ++m) {
            /* factorial */
            fact_seq[m] = fact_seq[m - 1] * (seq_t)m;

            /* term 1: 2 * Σ_{k=1}^{m-1} (m-1)! / k */
            seq_t term1 = (seq_t)0;
            if (m >= 2) {
                seq_t F = fact_seq[m - 1];
                for (idx_t k = 1; k <= m - 1; ++k) {
                    term1 += F / (seq_t)k;
                }
                term1 *= (seq_t)2;
            }

            /* term 2: Σ_{i=2}^{m-1} a[i] * C(m,i) * (m-i-1)! */
            seq_t term2 = (seq_t)0;
            for (idx_t i = 2; i <= m - 1; ++i) {
                /* C(m,i) = m! / (i! * (m-i)!) */
                seq_t binom = fact_seq[m] / fact_seq[i];
                /* binom * a[i] / (m-i) equals C(m,i) * (m-i-1)! * a[i]  */
                seq_t t = binom * a_seq[(size_t)i];
                t = t / (seq_t)(m - i);
                term2 += t;
            }

            a_seq[(size_t)m] = term1 + term2;
        }
        max_computed = n;
    }

    return a_seq[(size_t)n];
}
