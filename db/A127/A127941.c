/*
 * A127941  a(n) = numerator of b(n), where b(1)=1, b(n) = Sum_{1<=k<n,
 *          gcd(k,n)=1} 1/b(k).
 * https://oeis.org/A127941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) is the numerator of b(n), where b(1)=1 and for n>1
 *            b(n)=Σ_{1≤k<n, gcd(k,n)=1} 1/b(k)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps arrays of numerators and denominators
   and recomputes new terms on demand. It works for any seq_t and
   idx_t supplied by the build system.  No external floating‑point
   arithmetic is used. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"      /* provides static seq_t gcd(seq_t, seq_t) */

static seq_t *num_arr = NULL;   /* numerators of b(k) */
static seq_t *den_arr = NULL;   /* denominators of b(k) */
static size_t  cap     = 0;     /* allocated capacity */
static idx_t   filled  = 0;     /* number of values already known */

/* Euclidean GCD for idx_t (unsigned 64‑bit). */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Reduce a fraction a/b by their greatest common divisor. */
static inline void reduce_fraction(seq_t *a, seq_t *b)
{
    seq_t g = gcd(*a, *b);
    if (g > (seq_t)1) {
        *a /= g;
        *b /= g;
    }
}

/* Return the numerator a(n) of b(n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* undefined but harmless */

    /* Allocate or grow storage to contain at least n elements. */
    if (!num_arr || (size_t)n > cap) {
        size_t new_cap = cap ? cap << 1 : 4;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *new_num = (seq_t *)realloc(num_arr, new_cap * sizeof(seq_t));
        seq_t *new_den = (seq_t *)realloc(den_arr, new_cap * sizeof(seq_t));
        if (!new_num || !new_den) { /* allocation failed – abort */
            exit(1);
        }
        num_arr = new_num;
        den_arr = new_den;
        cap     = new_cap;
    }

    /* Compute terms up to the requested n. */
    for (idx_t m = filled + 1; m <= n; ++m) {
        if (m == 1) {
            num_arr[0] = (seq_t)1;
            den_arr[0] = (seq_t)1;
            continue;
        }

        seq_t sum_num = (seq_t)0;
        seq_t sum_den = (seq_t)1;

        for (idx_t k = 1; k < m; ++k) {
            if (gcd_idx(k, m) != 1) continue;

            /* 1/b(k) = den_arr[k-1] / num_arr[k-1] */
            seq_t inv_num = den_arr[k-1];
            seq_t inv_den = num_arr[k-1];

            /* sum = sum_num/sum_den + inv_num/inv_den */
            seq_t t1 = sum_num * inv_den;
            seq_t t2 = inv_num * sum_den;
            seq_t new_num = t1 + t2;
            seq_t new_den = sum_den * inv_den;

            reduce_fraction(&new_num, &new_den);

            sum_num = new_num;
            sum_den = new_den;
        }

        /* Store the reduced fraction for b(m). */
        reduce_fraction(&sum_num, &sum_den);
        num_arr[m-1] = sum_num;
        den_arr[m-1] = sum_den;
    }

    filled = n;
    return num_arr[n-1];
}
