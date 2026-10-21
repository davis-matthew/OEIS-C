/*
 * A054655  Triangle T(n,k) giving coefficients in expansion of
 *          n!*binomial(x-n,n) in powers of x.
 * https://oeis.org/A054655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy, nice
 * Formula:   a(n) = T(m, k) where m = floor((sqrt(8n+1)-1)/2) and k = n -
 *            m(m+1)/2. T(m,k) = (-1)^k * e_k(m, m+1, …, 2m-1), with e_k the
 *            k‑th elementary symmetric sum of the integers m, m+1, …, 2m-1.
 *            The triangle T is defined by n!*binomial(x-n,n) = Σ_{k=0..n}
 *            T(n,k) * x^(n-k)
 * Range:     i8 n <= 11, i16 n <= 24, i32 n <= 52, i64 n <= 132,
 *            i128 n <= 347, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes a(n) by finding the block
   (triangular) that contains n, then evaluating the required
   elementary symmetric sum using only O(m·k) arithmetic
   operations, which is well below the time limit for n up to 10 000. */
#include <stddef.h>   /* for size_t */

/* seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* Find the largest integer m such that m*(m+1)/2 <= n. */
    idx_t m = 0;
    while (((m + 1) * (m + 2)) / 2 <= n)
        ++m;

    /* t(m) = m*(m+1)/2 is the start index of the m‑th block. */
    idx_t t = m * (m + 1) / 2;
    idx_t k = n - t;                     /* 0 <= k <= m */

    /* e[0..k] will hold the elementary symmetric sums e_0 ... e_k. */
    /* 200 is safely larger than the maximum possible k (≈142 for n≤10 000). */
    seq_t e[200] = {0};
    e[0] = 1;

    /* Compute e_k for the set {m, m+1, …, 2m-1}. */
    if (m > 0 && k > 0) {
        for (idx_t num = m; num <= 2 * m - 1; ++num) {
            seq_t num_t = (seq_t)num;
            /* Update e[i] from high to low to avoid overwriting dependencies. */
            for (int i = (int)k; i >= 1; --i) {
                e[i] += num_t * e[i - 1];
            }
        }
    }

    /* Apply the sign (-1)^k. */
    if ((k & 1) == 0)
        return e[(int)k];
    else
        return -e[(int)k];
}
