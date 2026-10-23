/*
 * A175253  a(n) = characteristic function of numbers k such that A000203(m) =
 *          k has no solution for any m, where A000203(m) = sum of divisors of
 *          m.
 * https://oeis.org/A175253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if there is no m with A000203(m)=n (i.e. σ(m)=n), else
 *            0. The function is memoized and recomputes all σ values up to
 *            the largest n requested so far
 * Range:     u8 n <= 19, u16 n <= 2887, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static idx_t precomp_max = 0;
static bool   *seen       = NULL;   /* seen[x] == true if some σ(m) = x for m ≤ precomp_max */
static seq_t  *sigma      = NULL;   /* σ values for m ≤ precomp_max */
static idx_t  *spf        = NULL;   /* smallest prime factor table up to precomp_max */

/* Recompute data structures up to new_max (inclusive). */
static void recompute(idx_t new_max)
{
    if (new_max <= precomp_max)
        return;

    /* Resize arrays (or allocate once). */
    seen  = (bool *)realloc(seen,  (new_max + 1) * sizeof(bool));
    sigma = (seq_t *)realloc(sigma, (new_max + 1) * sizeof(seq_t));
    spf   = (idx_t *)realloc(spf,   (new_max + 1) * sizeof(idx_t));

    /* Initialize new bool entries to false. */
    if (precomp_max + 1 <= new_max) {
        memset(seen + precomp_max + 1, 0,
               (size_t)(new_max - precomp_max) * sizeof(bool));
    }

    /* Build smallest prime factor table up to new_max. */
    for (idx_t i = 0; i <= new_max; ++i)
        spf[i] = 0;
    for (idx_t i = 2; i <= new_max; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i * i <= new_max) {
                for (idx_t j = i * i; j <= new_max; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* Compute σ values incrementally. */
    if (new_max >= 1) {
        sigma[1] = (seq_t)1;
        seen[1]  = true;                  /* σ(1) = 1 */
    }

    for (idx_t m = 2; m <= new_max; ++m) {
        idx_t p = spf[m];
        idx_t e = 0;
        idx_t temp = m;
        while (temp % p == 0) {
            ++e;
            temp /= p;
        }
        idx_t rest = temp;          /* remaining part, < m */

        /* Compute (p^(e+1) - 1)/(p - 1) */
        seq_t power = (seq_t)1;
        for (idx_t k = 0; k < e + 1; ++k)
            power = power * (seq_t)p;
        seq_t part = (power - (seq_t)1) / (seq_t)(p - 1);

        sigma[m] = sigma[rest] * part;
        if (sigma[m] <= (seq_t)new_max)
            seen[(size_t)sigma[m]] = true;
    }

    precomp_max = new_max;
}

seq_t a(idx_t n)
{
    if (n == 0)      /* out of OEIS range, but return a conservative value */
        return (seq_t)0;

    recompute(n);
    return seen[(size_t)n] ? (seq_t)0 : (seq_t)1;
}
