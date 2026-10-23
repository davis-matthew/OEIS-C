/*
 * A164925  Array, binomial(j-i,j), read by rising antidiagonals.
 * https://oeis.org/A164925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = binomial(2*k - s, k) where s = floor((sqrt(8*n+1)-1)/2)
 *            and k = n - s*(s+1)/2. The binomial is the generalized binomial
 *            coefficient, defined for negative upper arguments by binom(-m,
 *            k) = (-1)^k * binom(m + k - 1, k)
 * Range:     i8 n <= 122, i16 n <= 383, i32 n <= 1287, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Compute binomial(n, k) for non‑negative n and 0 <= k <= n.
   Uses 128‑bit arithmetic to avoid overflow in intermediate steps.
   The final result is cast back to seq_t; if it does not fit,
   the overflow behaviour matches the underlying type. */
static inline seq_t binom_positive(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    __int128 result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (n - k + i) / i;
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    /* Find the diagonal index s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
    idx_t s = (idx_t)((sqrt(1.0 + 8.0 * (double)n) - 1.0) / 2.0);
    /* Correct possible rounding errors. */
    while ((seq_t)s * ((seq_t)s + 1) / 2 > n) s--;
    while (((seq_t)(s + 1) * ((seq_t)(s + 2)) / 2) <= n) s++;

    idx_t start = s * (s + 1) / 2;   /* first index on diagonal s */
    idx_t k     = n - start;         /* offset inside the diagonal */

    seq_t top = (seq_t)2 * (seq_t)k - (seq_t)s;   /* 2*k - s */

    if (top >= 0) {
        return binom_positive((idx_t)top, k);
    } else {
        /* For negative upper argument: binom(-m, k) = (-1)^k * binom(m + k - 1, k) */
        idx_t m   = (idx_t)(-top);
        seq_t res = binom_positive(m + k - 1, k);
        if (k & 1) res = -res;
        return res;
    }
}
