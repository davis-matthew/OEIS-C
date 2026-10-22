/*
 * A000763  Number of interval orders constructed from n intervals of generic
 *          lengths.
 * https://oeis.org/A000763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   E.g.f. E(x) with E'/E = y^2, y*(2-exp(x*y)) = 1. With y = Sum
 *            b(n) x^n/n!, w = x*y, P = y^2, Q = 2*y^2 - y = y*exp(w): y' =
 *            Q*w', w' = Sum (m+1)*b(m) x^m/m!, so b(0) = 1, b(n+1) =
 *            Sum_{k=0..n} C(n,k)*Q(k)*(n-k+1)*b(n-k), P(n) = Sum_{k=0..n}
 *            C(n,k)*b(k)*b(n-k), Q(n) = 2*P(n) - b(n), a(0) = 1, a(n+...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define A763_MAX 10010

static seq_t a763_b[A763_MAX + 2];
static seq_t a763_a[A763_MAX + 2];
static seq_t a763_P[A763_MAX + 2];
static seq_t a763_Q[A763_MAX + 2];
static seq_t a763_row[A763_MAX + 2];
static idx_t a763_done = 0;
static int a763_init = 0;

seq_t a(idx_t n)
{
    idx_t m, k;
    if (n > A763_MAX) return (seq_t)0;
    if (!a763_init) {
        a763_b[0] = 1;
        a763_a[0] = 1;
        a763_init = 1;
    }
    while (a763_done < n) {
        seq_t p, s, t;
        m = a763_done;
        /* Pascal row C(m, .) */
        a763_row[m] = 1;
        for (k = m; k > 1; k--)
            a763_row[k - 1] += a763_row[k - 2];
        /* P(m), Q(m) */
        p = 0;
        for (k = 0; k <= m; k++)
            p += a763_row[k] * a763_b[k] * a763_b[m - k];
        a763_P[m] = p;
        a763_Q[m] = p * 2 - a763_b[m];
        /* b(m+1) */
        s = 0;
        for (k = 0; k <= m; k++)
            s += a763_row[k] * a763_Q[k] * ((seq_t)(m - k + 1) * a763_b[m - k]);
        a763_b[m + 1] = s;
        /* a(m+1) */
        t = 0;
        for (k = 0; k <= m; k++)
            t += a763_row[k] * a763_a[k] * a763_P[m - k];
        a763_a[m + 1] = t;
        a763_done = m + 1;
    }
    return a763_a[n];
}
