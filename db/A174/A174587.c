/*
 * A174587  Triangle read by rows: T(n,k) = S(n,k) - S(n,0) + 1 where S(n,k) =
 *          Sum_{j=0..n} (-1)^j * (k-j)^n * Eulerian1(n+1, j) with S(0,k)=1.
 * https://oeis.org/A174587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, less
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2) and k = n -
 *            r(r+1)/2 T(r,k) = Sum_{j=0..r} (-1)^j*(k-j)^r * Eulerian(r+1,j)
 *            - Sum_{j=0..r} (-1)^j*(-j)^r * Eulerian(r+1,j) + 1
 *            (Eulerian(0,0)=1 and Eulerian(n,k)=0 for k>n)
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 36, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <math.h>

/* ---------- Eulerian numbers cache ---------- */
#define MAX_EULER 200                     /* enough for n up to ~140 */
static seq_t euler[MAX_EULER][MAX_EULER] = {0};
static int max_euler_n = 0;               /* largest n for which euler[n][*] is computed */

/* Precompute Eulerian numbers up to needed_n (inclusive).  Required
   for rows r+1 of the triangle.  euler[0][0] is fixed to 1. */
static void ensure_euler(int needed_n)
{
    if (needed_n <= max_euler_n)
        return;
    if (needed_n >= MAX_EULER)
        needed_n = MAX_EULER - 1;

    for (int n = max_euler_n + 1; n <= needed_n; ++n) {
        euler[n][0] = 1;
        for (int k = 1; k <= n; ++k) {
            seq_t term1 = (seq_t)(k + 1) * euler[n - 1][k];
            seq_t term2 = (seq_t)(n - k) * euler[n - 1][k - 1];
            euler[n][k] = term1 + term2;
        }
    }
    max_euler_n = needed_n;
}

/* Integer power: base^exp, exp >= 0.  Works for negative bases. */
static seq_t int_pow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1U)
            result *= base;
        exp >>= 1U;
        if (exp)
            base *= base;
    }
    return result;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Flattened index to triangle coordinates (r,k). */
    if (n == 0u) {
        return (seq_t)1;
    }

    /* Compute row r using the triangular number formula. */
    idx_t r = (idx_t)((sqrtl((long double)n * 8.0L + 1.0L) - 1.0L) / 2.0L);
    idx_t offset = r * (r + 1) / 2;
    while (offset > n) {
        --r;
        offset = r * (r + 1) / 2;
    }
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
        offset = r * (r + 1) / 2;
    }

    idx_t k = n - offset;   /* column within the row */

    ensure_euler((int)(r + 1));

    seq_t sum0 = 0;  /* S(r,0) */
    seq_t sumk = 0;  /* S(r,k) */

    for (idx_t j = 0; j <= r; ++j) {
        bool negative = (j & 1U);
        seq_t coeff = euler[(int)(r + 1)][(int)j];

        /* S(r,0) term: (0 - j)^r = (-j)^r */
        seq_t pow0 = int_pow((seq_t)(- (seq_t)j), r);
        seq_t term0 = pow0 * coeff;
        if (negative)
            term0 = -term0;
        sum0 += term0;

        /* S(r,k) term: (k - j)^r */
        seq_t powk = int_pow((seq_t)k - (seq_t)j, r);
        seq_t termk = powk * coeff;
        if (negative)
            termk = -termk;
        sumk += termk;
    }

    /* Final value: T(r,k) = S(r,k) - S(r,0) + 1 */
    return sumk - sum0 + (seq_t)1;
}
