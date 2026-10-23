/*
 * A120306  Numerator of the sum of all matrix elements of n X n matrix
 *          M[i,j]=CatalanNumber[i]/CatalanNumber[j], where
 *          CatalanNumber[k]=(2k)!/k!/(k+1)!=A000108[k].
 * https://oeis.org/A120306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = Numerator[ Σ_{i=1}^n Σ_{j=1}^n Catalan[i]/Catalan[j] ]
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   catalan, gcd, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/catalan.h"
#include "../utils/gcd.h"
#include "../utils/seq_realloc.h"

/* cached data: cumulative sums of Catalan numbers, and of 1/Catalan
   as a reduced fraction numerator/denominator. */
static seq_t *cum_C  = NULL;   /* cum_C[i] = Σ_{k=1}^i Catalan(k) */
static seq_t *num_B  = NULL;   /* numerator of Σ_{k=1}^i 1/Catalan(k) */
static seq_t *den_B  = NULL;   /* denominator   of Σ_{k=1}^i 1/Catalan(k) */
static size_t cached_len = 0;  /* current allocated length */
static idx_t   max_cached = 0; /* largest n computed so far */

/* extend the caches to hold values up to n (inclusive) */
static void ensure_cached(idx_t n)
{
    if (n <= max_cached) return;

    size_t new_cap = (size_t)(n + 1);
    if (cached_len < new_cap) {
        cum_C = seq_realloc(cum_C, cached_len, new_cap);
        num_B = seq_realloc(num_B, cached_len, new_cap);
        den_B = seq_realloc(den_B, cached_len, new_cap);
        cached_len = new_cap;
    }

    /* initialise first entries on first allocation */
    if (max_cached == 0) {
        cum_C[0] = (seq_t)0;
        num_B[0] = (seq_t)0;
        den_B[0] = (seq_t)1;
    }

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t C = catalan(i);

        /* cumulative Catalan sum */
        cum_C[i] = cum_C[i - 1] + C;

        /* update running sum of reciprocals */
        seq_t prev_num = num_B[i - 1];
        seq_t prev_den = den_B[i - 1];

        seq_t n_num = prev_num * C + prev_den; /* prev_num/prev_den + 1/C */
        seq_t n_den = prev_den * C;

        seq_t g = gcd(n_num, n_den);
        n_num = n_num / g;
        n_den = n_den / g;

        num_B[i] = n_num;
        den_B[i] = n_den;
    }

    max_cached = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    ensure_cached(n);

    seq_t S   = cum_C[n];   /* Σ Catalan(k) */
    seq_t num = num_B[n];   /* numerator of Σ 1/Catalan(k) */
    seq_t den = den_B[n];   /* denominator   of Σ 1/Catalan(k) */

    seq_t prod = S * num;          /* numerator of the raw product */
    seq_t g    = gcd(prod, den);   /* reduce the fraction */
    return prod / g;              /* a(n) is the reduced numerator */
}
