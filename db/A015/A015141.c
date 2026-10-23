/*
 * A015141  Triangle of (Gaussian) q-binomial coefficients for q=-17.
 * https://oeis.org/A015141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) is the anti‑diagonal term of the table of Gaussian
 *            q‑binomial coefficients with q = -17. For the row index r and
 *            column index k (0 ≤ k ≤ r) defined by r*(r+1)/2 ≤ n <
 *            (r+1)*(r+2)/2 and k = n - r*(r+1)/2, the value is a(n) =
 *            binom_q(-17, r, k). The product formula used is binom_q(q, r, k)
 *            = ∏...
 * Range:     i8 n <= 3, i16 n <= 10, i32 n <= 22, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* utilities provided by the harness */
#include <stddef.h>          /* size_t */
#include <stdlib.h>         /* malloc, free */
#include "../utils/gcd.h"   /* gcd(seq_t,seq_t) */

/* Find the largest r such that r*(r+1)/2 <= n. */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Exponential search to bracket the answer. */
    while (((__int128)high * (high + 1)) / 2 <= n)
        high <<= 1;
    /* Binary search within the bracket. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__int128)mid * (mid + 1)) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;  /* row index r */
}

/* Gaussian q-binomial coefficient for q = -17. */
static seq_t q_binom(idx_t n, idx_t k)
{
    if (k == 0 || k == n)
        return 1;

    if (k > n - k)
        k = n - k;   /* use symmetry */

    /* Pre‑compute powers q^i for i = 0..n. */
    seq_t *pow = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!pow)
        return 0;  /* fallback if allocation fails */
    pow[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow[i] = pow[i - 1] * (seq_t)-17;

    seq_t result = 1;
    for (idx_t i = 0; i < k; ++i) {
        seq_t num = 1 - pow[n - i];   /* 1 - q^(n-i) */
        seq_t den = 1 - pow[i + 1];   /* 1 - q^(i+1) */

        /* Reduce the fraction to keep numbers small. */
        seq_t g1 = gcd(num, den);
        num /= g1;
        den /= g1;

        seq_t g2 = gcd(result, den);
        result /= g2;
        den /= g2;

        result *= num;   /* now divisible by den */
        if (den != (seq_t)1)
            result /= den;
    }

    free(pow);
    return result;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t j = n - (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    return q_binom(r, j);
}
