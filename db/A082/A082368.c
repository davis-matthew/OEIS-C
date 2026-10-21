/*
 * A082368  a(n) = (4*n-1)! / (n! * n! * n! * (n-1)! * 3!).
 * https://oeis.org/A082368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*(2*n-2+1)*(4*n-2+1)*(4*n-2+3)*a(n‑1)/(n)^3 with a(1) =
 *            1. Recurrence: a(n) = a(n‑1) * 8 * (2n‑3) * (4n‑5) * (4n‑3) /
 *            n^3 (for n ≥ 2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* gcd for seq_t – works for both integral and arbitrary‑precision seq_t. */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Memoised recurrence – works for any order of calls. */
static seq_t *cache = NULL;        /* cache[1] = a(1) */
static idx_t cache_size = 0;       /* allocated size (>= stored index+1) */

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;               /* undefined for n < 1 */

    /* Expand cache if necessary */
    if (cache_size < n + 1) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;           /* allocation failure – unlikely */
        cache = tmp;
        /* initialise new entries to 0 */
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* Base case */
    if (cache[1] == (seq_t)0)
        cache[1] = (seq_t)1;

    /* Compute values up to n if missing */
    for (idx_t k = 2; k <= n; ++k) {
        if (cache[k] != (seq_t)0)
            continue;   /* already computed */

        seq_t prev = cache[k - 1];

        /* Factors for recurrence */
        seq_t numer[4];
        numer[0] = (seq_t)8;
        numer[1] = (seq_t)(2 * (k - 1) + 1);     /* 2n‑3 */
        numer[2] = (seq_t)(4 * (k - 1) + 1);     /* 4n‑5 */
        numer[3] = (seq_t)(4 * (k - 1) + 3);     /* 4n‑3 */

        /* Denominator: k^3 */
        seq_t denom[3];
        denom[0] = denom[1] = denom[2] = (seq_t)k;

        seq_t res = prev;

        /* For each numerator factor */
        for (int i = 0; i < 4; ++i) {
            seq_t f = numer[i];
            if (f == (seq_t)1)
                continue;

            /* Cancel common factors between f and denominator */
            for (int j = 0; j < 3 && f != (seq_t)1; ++j) {
                if (denom[j] == (seq_t)1)
                    continue;
                seq_t g = gcd_seq(f, denom[j]);
                if (g > (seq_t)1) {
                    f /= g;
                    denom[j] /= g;
                }
            }

            /* Cancel common factors between current result and remaining denom */
            for (int j = 0; j < 3 && denom[j] != (seq_t)1; ++j) {
                seq_t g = gcd_seq(res, denom[j]);
                if (g > (seq_t)1) {
                    res /= g;
                    denom[j] /= g;
                }
            }

            /* Multiply result by the reduced numerator factor */
            res = res * f;

            /* Cancel any remaining common factors between result and denom */
            for (int j = 0; j < 3 && denom[j] != (seq_t)1; ++j) {
                seq_t g = gcd_seq(res, denom[j]);
                if (g > (seq_t)1) {
                    res /= g;
                    denom[j] /= g;
                }
            }
        }

        /* Any remaining denominator factors should divide the result */
        for (int j = 0; j < 3; ++j) {
            if (denom[j] != (seq_t)1)
                res = res / denom[j];
        }

        cache[k] = res;
    }

    return cache[n];
}
