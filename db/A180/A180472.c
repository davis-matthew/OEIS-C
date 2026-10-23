/*
 * A180472  Triangle T(n, k) = OC(n, k; not -1), read by rows, where OC(n, k;
 *          not -1) is the number of k-subsets of Z_n without -1 as a
 *          multiplier, up to congruency.
 * https://oeis.org/A180472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle read by rows, * where
 *            T(n,k) = OC(n,k;not-1) is computed by * T(n,k) =
 *            -binomial(floor(n/2) - (k & 1) * (1 - (n & 1)), * (k/2)) / 2 * +
 *            Sum_{d|n, d|k} (phi(d)*binomial(n/d,k/d)) / (2*n) * * The
 *            function below follows this formula directly, using the *
 *            prov...
 * Range:     u8 n <= 39, u16 n <= 142, u32 n <= 580, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   binomial, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"   /* binomial(idx_t, idx_t) -> seq_t   */
#include "../utils/phi.h"        /* phi(idx_t) -> seq_t             */

static seq_t T_val(idx_t n, idx_t k)
{
    /* --- first summand: -binomial(...) / 2 ----------------------------- */
    idx_t a = n / 2;                     /* floor(n/2)                */
    if (((k & 1ULL) != 0) && ((n & 1ULL) == 0))
        a--;                             /* subtract 1 when n even & k odd */
    idx_t b = k / 2;                     /* floor(k/2)                */
    seq_t bin = binomial(a, b);          /* choose(a,b)                */
    seq_t bin_div2 = bin / 2;            /* integer division           */

    /* --- second summand: Sum_{d|n, d|k} (phi(d)*binomial(n/d,k/d)) / (2*n) */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;       /* skip non‑divisors        */
        /* first divisor d */
        if (k % d == 0) {
            seq_t phi_d = phi(d);
            idx_t nd = n / d;
            idx_t kd = k / d;
            seq_t bin_nd = binomial(nd, kd);
            sum += phi_d * bin_nd;
        }
        /* paired divisor n/d */
        idx_t d2 = n / d;
        if (d2 != d && k % d2 == 0) {
            seq_t phi_d2 = phi(d2);
            idx_t nd2 = d;          /* since n/d2 == d             */
            idx_t kd2 = k / d2;
            seq_t bin_nd2 = binomial(nd2, kd2);
            sum += phi_d2 * bin_nd2;
        }
    }

    /* compute the two parts and combine */
    seq_t denom = 2 * (seq_t)n;          /* 2*n as seq_t                */
    seq_t sum_term = sum / denom;        /* exact division by theory     */
    seq_t result = sum_term;             /* start with positive part     */
    result -= bin_div2;                  /* subtract the negative part   */

    return result;
}

/* a(n) – the n‑th term of the triangle read by rows. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* first element of the triangle */

    /* locate row r so that the first index of row r is r*(r+1)/2 */
    idx_t r = 0;
    idx_t prefix = 0;                    /* sum of lengths of previous rows */
    while (prefix + (r + 1) <= n) {
        prefix += (r + 1);
        ++r;
    }

    idx_t k = n - prefix;                /* column within row r */
    return T_val(r, k);
}
