/*
 * A155452  Triangle read by rows: let t1(n,k)=Sum[(-1)^j Binomial[n + 1, j](k
 *          + 1 - j)^n, {j, 0, k + 1}]; then T(n,m)=2*t1(n + 1, k) - (m! - n!
 *          + (-m + n)!).
 * https://oeis.org/A155452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,m) = 2*∑_{j=0}^{m+1} (-1)^j*C(n+2,j)*(m+1-j)^{n+1} – (m! –
 *            n! + (n-m)!) a(k) is the triangle read by rows; offset(k) =
 *            k*(k+1)/2
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* -------------------------------------------------------------------------- */
/*  Helper: binomial N choose K, computed multiplicatively to avoid large   */
/*  intermediate values (for small N this works on any seq_t).               */
/* -------------------------------------------------------------------------- */
static seq_t binom_calc(idx_t N, idx_t K) {
    if ((idx_t)K > N) return 0;
    if (K > N - K) K = N - K;
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        /* multiply then divide; the division is exact for integers */
        res = res * (seq_t)(N - K + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* -------------------------------------------------------------------------- */
/*  Helper: fast exponentiation of a seq_t base to idx_t exponent.             */
/* -------------------------------------------------------------------------- */
static seq_t seq_pow(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

/* -------------------------------------------------------------------------- */
/*  Helper: factorial of n (non‑negative).  n is small enough for a 64‑bit   */
/*  temporary running product; the final result is stored in seq_t.            */
/* -------------------------------------------------------------------------- */
static seq_t fact(idx_t n) {
    seq_t r = 1;
    for (idx_t i = 2; i <= n; ++i) r = r * (seq_t)i;
    return r;
}

/* -------------------------------------------------------------------------- */
/*  Compute a single triangle element T(n,m).  n and m are 0‑based and       */
/*  m ≤ n.                                                                */
/* -------------------------------------------------------------------------- */
static seq_t triangle_T(idx_t n, idx_t m) {
    seq_t sum = 0;
    idx_t exp = n + 1;           /* exponent in t1 definition */
    idx_t upper = m + 1;         /* j runs from 0 to m+1 */

    for (idx_t j = 0; j <= upper; ++j) {
        seq_t bin = binom_calc(n + 2, j);
        seq_t base = (seq_t)(m + 1 - j);
        seq_t p = seq_pow(base, exp);
        seq_t term = bin * p;
        if (j & 1) sum -= term;
        else       sum += term;
    }

    seq_t two_sum = sum * 2;
    seq_t diff = fact(m) - fact(n) + fact(n - m);
    return two_sum - diff;
}

/* -------------------------------------------------------------------------- */
/*  Main function required by the specification.  Computes a(k) by locating   */
/*  the appropriate row and column in the triangle and evaluating T(n,m).    */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t k) {
    /* Find row r such that r*(r+1)/2 ≤ k < (r+1)*(r+2)/2 */
    idx_t r = (idx_t)((sqrt(8.0 * k + 1.0) - 1.0) / 2.0);
    /* Adjust for possible floating point rounding errors */
    while ((r + 1) * (r + 2) / 2 <= k) ++r;
    while (r * (r + 1) / 2 > k) --r;

    idx_t offset = r * (r + 1) / 2;
    idx_t m = k - offset;

    return triangle_T(r, m);
}
