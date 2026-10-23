/*
 * A128191  a(n) = numerator of b(n), where b(1)=1, b(n+1) = sum{k=1 to n}
 *          {b(n+1-k)/k} ({x} is the fractional part of x).
 * https://oeis.org/A128191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = numerator of b(n); 
 * b(1) = 1; b(n+1) = sum_{k=1}^{n} {b(n+1-k) / k}  ({x} is the fractional part of x). 
 * a(n) is defined for n ≥ 1. 
 */

#include <stdlib.h>
#include "../utils/gcd.h"

/* Arrays holding the numerators and denominators of b(k) for k = 1..cap (1‑based). */
static seq_t *num = NULL;
static seq_t *den = NULL;
static idx_t   cap = 0;   /* allocated capacity */
static idx_t   sz  = 0;   /* number of terms computed */

/* Ensure storage for at least n terms. */
static void
ensure_capacity(idx_t n)
{
    if (n <= cap)
        return;
    idx_t newcap = cap ? (cap << 1) : 1;
    while (newcap < n)
        newcap <<= 1;
    seq_t *newnum = (seq_t *)malloc(newcap * sizeof(seq_t));
    seq_t *newden = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!newnum || !newden)
        abort();
    for (idx_t i = 0; i < sz; ++i) {
        newnum[i] = num[i];
        newden[i] = den[i];
    }
    free(num);
    free(den);
    num = newnum;
    den = newden;
    cap = newcap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for n = 0, return 0 for safety */

    if (n <= sz)
        return num[n - 1];               /* already computed */

    ensure_capacity(n);

    for (idx_t i = sz + 1; i <= n; ++i) {
        if (i == 1) {                    /* base case: b(1)=1/1 */
            num[0] = (seq_t)1;
            den[0] = (seq_t)1;
            continue;
        }

        /* Compute b(i) as the sum over k = 1 .. i-1 of fractional parts of b(i-k)/k. */
        seq_t sum_num = (seq_t)0;
        seq_t sum_den = (seq_t)1;

        for (idx_t k = 1; k < i; ++k) {
            idx_t j = i - k;                     /* index of b(j) */
            seq_t p = num[j - 1];                /* numerator of b(j) */
            seq_t q = den[j - 1];                /* denominator of b(j) */

            seq_t k_seq   = (seq_t)k;            /* convert k to seq_t */
            seq_t denom_k = q * k_seq;           /* q * k  */
            seq_t rem     = p % denom_k;         /* p mod (q*k) */

            /* Add the fraction rem / denom_k to the running sum. */
            seq_t g  = gcd(sum_den, denom_k);           /* gcd of denominators */
            seq_t sum_den_div_g = sum_den / g;
            seq_t denom_k_div_g = denom_k / g;

            seq_t new_num = sum_num * denom_k_div_g + rem * sum_den_div_g;
            seq_t new_den = sum_den_div_g * denom_k;

            seq_t g2 = gcd(new_num, new_den);          /* reduce */
            sum_num = new_num / g2;
            sum_den = new_den / g2;
        }

        num[i - 1] = sum_num;
        den[i - 1] = sum_den;
    }

    sz = n;
    return num[n - 1];
}
