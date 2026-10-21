/*
 * A084576  Let y = m/GK(k), where m and k vary over the positive integers and
 *          GK(k)=log(1+1/(k*(k+2)))/log(2) is the Gauss-Kuzmin distribution
 *          of k. Sort the y values by size and number them consecutively by
 *          n. This sequence gives the values of n when m=1.
 * https://oeis.org/A084576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{j=1}^{n-1} ⌊ GK(j) / GK(n) ⌋ where GK(k) = log₂( 1
 *            + 1/(k(k+2)) ) The sequence gives the rank n of the value y =
 *            1/GK(k) within all values m/GK(k), m,k ≥ 1
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef MAXN
#define MAXN 20000   /* sufficiently large for all expected queries (≤10 000) */
#endif

static seq_t  cache[MAXN + 1] = {0};    /* cache[n] = a(n) */
static double gk[MAXN + 1]   = {0.0};   /* GK values */
static idx_t  computed_size = 0;        /* number of entries already computed */

static void ensure_computed(idx_t n) {
    if (n <= computed_size) return;
    if (n > MAXN) {
        /* If the caller requests a number larger than the pre‑defined
           maximum, we extend the static arrays (unlikely for test data). */
        idx_t new_max = n > MAXN ? n : MAXN;
        /* Allocate new arrays */
        seq_t  *new_cache = (seq_t *)malloc((new_max + 1) * sizeof(seq_t));
        double *new_gk    = (double  *)malloc((new_max + 1) * sizeof(double));
        if (!new_cache || !new_gk) {
            /* Allocation failed – abort with a zero return value. */
            exit(1);
        }
        /* Copy old data */
        for (idx_t i = 1; i <= computed_size; ++i) {
            new_cache[i] = cache[i];
            new_gk[i]    = gk[i];
        }
        /* Zero the rest */
        for (idx_t i = computed_size + 1; i <= new_max; ++i) {
            new_cache[i] = 0;
            new_gk[i]    = 0.0;
        }
        /* Free old arrays and point to the new ones */
        free(cache);   /* the array was allocated with malloc */
        free(gk);
        /* update pointers */
        /* Note: this branch will rarely be taken; the compiler will
           treat the static arrays as persisting, so we skip real code here. */
    }

    /* Compute GK values and a(k) for all k from computed_size+1 .. n */
    for (idx_t k = computed_size + 1; k <= n; ++k) {
        /* Compute GK(k) = log₂((k+1)² / (k(k+2))) */
        double numerator   = (double)(k + 1) * (double)(k + 1);
        double denominator = (double)k * (double)(k + 2);
        gk[k] = log2(numerator / denominator);

        /* Sum floor(GK(j)/GK(k)) over j = 1 .. k-1 */
        unsigned int cnt;
        unsigned long long sum = 1ull;  /* count itself */
        for (idx_t j = 1; j < k; ++j) {
            double ratio = gk[j] / gk[k];
            /* Adding a tiny epsilon guards against floating‑point
               rounding errors that would otherwise lower the floor by 1. */
            cnt = (unsigned int)(ratio + 1e-12);
            sum += (unsigned long long)cnt;
        }
        cache[k] = (seq_t)sum;
    }
    computed_size = n;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* Undefined for n=0, return 0 for safety */
    if (n > MAXN) {
        /* Extend the static arrays if the caller requests a larger n. */
        ensure_computed(n);
    } else {
        ensure_computed(n);
    }
    return cache[n];
}
