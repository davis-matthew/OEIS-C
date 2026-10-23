/*
 * A009667  E.g.f. tan(sin(x))*x, even numbered terms only.
 * https://oeis.org/A009667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*n * c[n] (c[n] = (2n-1)! * [x^(2n-1)] tan(sin(x)))
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 12, i128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the differential equation for tan(sin(x)). */

#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *c_arr = NULL;          /* c[n]  */
static seq_t *m_arr = NULL;          /* m_even[q] = m[2q]  */
static size_t cap = 0;               /* allocated size of arrays */
static idx_t  computed = 0;          /* largest n that has been computed */

/* Ensure that c_arr[1..n] and m_even[0..n] are filled. */
static void ensure_cache(idx_t n)
{
    if (n <= computed) return;

    /* Allocate or grow storage as needed. */
    size_t new_cap = (cap == 0) ? 16 : cap;
    while (new_cap < (size_t)n + 1) new_cap <<= 1;
    c_arr = (seq_t *)realloc(c_arr, (new_cap + 1) * sizeof(seq_t));
    m_arr = (seq_t *)realloc(m_arr, (new_cap + 1) * sizeof(seq_t));
    cap = new_cap;

    /* Initialize base values if this is the first call. */
    if (computed == 0) {
        m_arr[0] = (seq_t)1;        /* m[0] = 1 */
        if (n >= 1) {
            c_arr[1] = (seq_t)1;    /* c[1] = 1 */
            /* m[2] = binom(2,1) * c1 * c1 */
            seq_t sum = (seq_t)0;
            seq_t bin = binom((idx_t)2, (idx_t)1);   /* = 2 */
            sum += bin * c_arr[1] * c_arr[1];
            m_arr[1] = sum;
        }
        computed = (n >= 1 ? 1 : 0);
    }

    /* Compute values for k = computed+1 .. n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        /* ---------- compute c[k] ---------- */
        seq_t ck = (seq_t)0;
        /* First sum: Σ_{q=0}^{k-1} (-1)^{k-q-1} * binom(2k-2, 2q) * m[2q] */
        seq_t bin = (seq_t)1;                     /* binom(2k-2,0) */
        idx_t r = 0;                              /* current r = 2*q */
        idx_t n1 = 2 * k - 2;
        for (idx_t q = 0; q < k; ++q) {
            seq_t term = bin * m_arr[q];
            if (((k - q - 1) & 1) != 0) {
                ck -= term;
            } else {
                ck += term;
            }
            /* update bin to binom(2k-2, 2q+2) if needed */
            if (q + 1 < k) {
                /* bin := bin * (n1 - r) * (n1 - r - 1) / ((r + 1)*(r + 2)) */
                bin *= (seq_t)(n1 - r);
                bin /= (seq_t)(r + 1);
                bin *= (seq_t)(n1 - r - 1);
                bin /= (seq_t)(r + 2);
                r += 2;
            }
        }
        c_arr[k] = ck;

        /* ---------- compute m[2k] ---------- */
        seq_t sum_m = (seq_t)0;
        /* Second sum: Σ_{x=1}^{k} binom(2k, 2x-1) * c[x] * c[k-x+1] */
        seq_t bin2 = (seq_t)(2 * k);              /* binom(2k,1) */
        idx_t r2 = 1;                              /* current r = 2x-1 */
        idx_t n2 = 2 * k;
        for (idx_t x = 1; x <= k; ++x) {
            seq_t t = c_arr[x] * c_arr[k - x + 1];
            t *= bin2;
            sum_m += t;
            /* update bin2 to binom(2k, 2x+1) if needed */
            if (x < k) {
                /* bin2 := bin2 * (n2 - r2) * (n2 - r2 - 1) / ((r2 + 1)*(r2 + 2)) */
                bin2 *= (seq_t)(n2 - r2);
                bin2 /= (seq_t)(r2 + 1);
                bin2 *= (seq_t)(n2 - r2 - 1);
                bin2 /= (seq_t)(r2 + 2);
                r2 += 2;
            }
        }
        m_arr[k] = sum_m;
    }

    computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    ensure_cache(n);

    seq_t factor = ((seq_t)n) * 2;   /* 2*n as seq_t */
    return factor * c_arr[n];
}
