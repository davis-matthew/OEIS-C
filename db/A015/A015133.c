/*
 * A015133  Triangle of (Gaussian) q-binomial coefficients for q=-15.
 * https://oeis.org/A015133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   T(n,k) = Π_{i=1..k} ((-15)^(n-i+1)-1)/((-15)^i-1) Sequence a(n)
 *            is the flattening (row‑major) of this triangle for n ≥ 0 (offset
 *            0)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 16, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *qpow = NULL;     /* qpow[i] = (-15)^i, i ≥ 0 */
static idx_t qp_len = 0;       /* number of computed powers   */
static idx_t qp_cap = 0;       /* current capacity of qpow    */

static void ensure_powers(idx_t n)
{
    if (n < qp_len)
        return;
    idx_t newcap = qp_cap ? qp_cap << 1 : 16;
    while (newcap <= n)
        newcap <<= 1;
    seq_t *tmp = (seq_t*)realloc(qpow, (size_t)newcap * sizeof(seq_t));
    if (!tmp)
        abort();
    qpow = tmp;
    qp_cap = newcap;
    if (qp_len == 0) {
        qpow[0] = 1;
        qp_len = 1;
    }
    for (idx_t i = qp_len; i <= n; ++i)
        qpow[i] = qpow[i-1] * (seq_t)(-15);
    qp_len = n + 1;
}

seq_t a(idx_t n)
{
    /* Find row r with indices from r*(r+1)/2 to r*(r+1)/2 + r. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t before = r * (r + 1) / 2;   /* first index of row r   */
    idx_t k = n - before;             /* column position         */

    ensure_powers(r);

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = qpow[r - i + 1] - 1;   /* (-15)^(r-i+1)-1 */
        seq_t den = qpow[i] - 1;           /* (-15)^i-1       */
        res = res * num;
        res = res / den;                   /* exact division  */
    }
    return res;
}
