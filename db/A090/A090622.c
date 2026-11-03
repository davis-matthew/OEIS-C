/*
 * A090622  Square array read by antidiagonals of highest power of k dividing
 *          n! (with n,k>1).
 * https://oeis.org/A090622
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = exponent of k dividing n! where (n,k) are taken by
 *            reading the infinite square array of the largest power of k
 *            dividing n! along anti‑diagonals starting at (n,k) = (2,2). The
 *            array is read by increasing sums s = n+k; for a given s the
 *            pairs (n,k) with n≥2,k≥2 and n+k = s appear in the orde...
 * Range:     u8 n <= 10012, u16 n <= 10012, u32 n <= 10012, u64 n <= 10012,
 *            u128 n <= 10012, big any n
 * Validated: standard, pass, 121 terms through n = 8929 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the exponent of k in n! (Legendre’s formula) */
static seq_t exp_k_in_factorial(idx_t n, idx_t k)
{
    /* Find the minimum over prime powers dividing k */
    idx_t temp = k;
    idx_t min_exp = UINT64_MAX;      /* hold the minimum in idx_t */

    /* Factor k by trial division (k is always ≥2) */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p != 0)
            continue;
        idx_t exp_k = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp_k;
        }
        /* Compute v_p(n!) via Legendre's formula */
        idx_t v = 0;
        idx_t t = n;
        while (t) {
            t /= p;
            v += t;
        }
        idx_t cand = v / exp_k;
        if (cand < min_exp)
            min_exp = cand;
    }
    /* Remaining prime factor (if any) */
    if (temp > 1) {
        idx_t p = temp;
        idx_t exp_k = 1;
        idx_t v = 0;
        idx_t t = n;
        while (t) {
            t /= p;
            v += t;
        }
        idx_t cand = v / exp_k;      /* exp_k is 1 here */
        if (cand < min_exp)
            min_exp = cand;
    }
    return (seq_t)min_exp;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* values below the offset are irrelevant */

    /* Find the anti‑diagonal sum s and the position within that sum. */
    idx_t m = n - 2;                   /* zero‑based index after the first term */
    idx_t s = 4;                       /* first sum: 2+2 */
    idx_t cum = 1;                     /* terms up to the current sum s */

    while (cum <= m) {
        ++s;
        cum += s - 3;                  /* number of terms in sum s */
    }

    idx_t cum_before = cum - (s - 3);  /* total terms before this sum */
    idx_t p = m - cum_before;          /* position within current sum, 0‑based */
    idx_t row = 2 + p;                 /* n in T(n,k) */
    idx_t col = s - row;               /* k in T(n,k) */

    return exp_k_in_factorial(row, col);
}
