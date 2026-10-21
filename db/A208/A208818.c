/*
 * A208818  Number of n-bead necklaces labeled with numbers -1..1 allowing
 *          reversal, with sum zero.
 * https://oeis.org/A208818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside counting for n‑bead necklaces with labels –1,0,1, *
 *            allowing reflection, with total sum 0. * a(n) = (1/(2n))*(
 *            Σ_{d|n} φ(n/d)·Z(d) + R(n) ), * where Z(t) = number of
 *            assignments of t variables from {‑1,0,1} * with sum 0, and R(n)
 *            counts colorings fixed by reflections * (see code comments)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes required for helper binom and standard types */
#include <stddef.h>                /* for size_t */
#include "../utils/binom.h"        /* binom(idx_t a, idx_t b) */

/* ---------- Euler's totient (small values) ---------- */
static seq_t phi(idx_t m)
{
    if (m <= 1) return (seq_t)1;
    seq_t res = (seq_t)m;
    idx_t tmp = m;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            res = res / (seq_t)p * (seq_t)(p - 1);
        }
    }
    if (tmp > 1) res = res / (seq_t)tmp * (seq_t)(tmp - 1);
    return res;
}

/* ---------- Count of assignments of t variables from {–1,0,1} with sum 0 ---------- */
static seq_t count_zero(idx_t t)
{
    seq_t total = (seq_t)0;
    for (idx_t a = 0; a <= t / 2; ++a) {
        seq_t c1 = binom(t, a);
        seq_t c2 = binom(t - a, a);
        total += c1 * c2;
    }
    return total;
}

/* ---------- Count of assignments of m variables from {–1,0,1} with sum s (s∈{–1,0,1}) ---------- */
static seq_t count_sum(idx_t m, int s)
{
    seq_t total = (seq_t)0;
    /* iterate over number of –1’s (y) */
    for (idx_t y = 0; y <= m; ++y) {
        int64_t xv = (int64_t)y + (int64_t)s;       /* x = y + s */
        if (xv < 0) continue;
        idx_t x = (idx_t)xv;
        if (x > m) continue;
        if (x + y > m) continue;                /* ensures z≥0 */
        seq_t c1 = binom(m, x);
        seq_t c2 = binom(m - x, y);
        total += c1 * c2;
    }
    return total;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    /* ---------- Rotations ---------- */
    /* rot_total = Σ_{d|n} φ(n/d) · Z(d)  */
    seq_t rot_total = (seq_t)0;
    /* gather divisors */
    static idx_t divs[200];   /* τ(n) ≤ 200 for n ≤ 10000 */
    size_t dcnt = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[dcnt++] = i;
            if (i * i != n) divs[dcnt++] = n / i;
        }
    }
    for (size_t k = 0; k < dcnt; ++k) {
        idx_t d = divs[k];
        seq_t ph = phi(n / d);
        seq_t z  = count_zero(d);
        rot_total += ph * z;
    }

    /* ---------- Reflections ---------- */
    seq_t refl_total = (seq_t)0;
    if (n & 1) {   /* odd n: all reflections have one fixed bead */
        idx_t m = (n - 1) / 2;
        refl_total = n * count_zero(m);
    } else {       /* even n: two types of reflections */
        /* Type A: axis through opposite beads (two fixed) */
        idx_t m1 = (n - 2) / 2;
        seq_t c1   = count_sum(m1,  1);  /* fixed sum = 2 → required pair sum = -1 */
        seq_t c0   = count_sum(m1,  0);  /* fixed sum = 0 → required pair sum = 0  */
        seq_t c_1  = count_sum(m1, -1);  /* fixed sum = -2 → required pair sum = 1  */
        seq_t reflA = (n / 2) * (c1 + (seq_t)3 * c0 + c_1);

        /* Type B: axis through gaps (no fixed) */
        idx_t m2 = n / 2;
        seq_t reflB = (n / 2) * count_sum(m2, 0);
        refl_total = reflA + reflB;
    }

    seq_t total = rot_total + refl_total;
    seq_t denom = (seq_t)(2 * n);
    return total / denom;
}
