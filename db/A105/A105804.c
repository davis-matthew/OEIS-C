/*
 * A105804  a(n)=F(n-1)a(n-1)+F(n)a(n-2), where F = A000045 (Fibonacci
 *          numbers), a(0)=0, a(1)=1.
 * https://oeis.org/A105804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n-1)*a(n-1) + F(n)*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static caches for the sequence and the Fibonacci numbers. */
static seq_t *a_cache = NULL;   /* a_cache[i] = a(i)          */
static idx_t a_cache_max = 0;   /* highest index computed in a_cache */
static seq_t *f_cache = NULL;   /* f_cache[i] = Fibonacci F(i) */
static idx_t f_cache_max = 0;   /* highest index computed in f_cache */

/* Compute a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    /* Initialise the caches on the first call. */
    if (a_cache == NULL) {
        a_cache = (seq_t*)malloc(sizeof(seq_t) * 2);
        if (!a_cache) return 0;          /* allocation failed */
        a_cache[0] = 0;
        a_cache[1] = 1;
        a_cache_max = 1;
    }
    if (f_cache == NULL) {
        f_cache = (seq_t*)malloc(sizeof(seq_t) * 2);
        if (!f_cache) return 0;          /* allocation failed */
        f_cache[0] = 0;
        f_cache[1] = 1;
        f_cache_max = 1;
    }

    /* Return immediately if the value is already computed. */
    if (n <= a_cache_max) {
        return a_cache[n];
    }

    /* Ensure the Fibonacci cache is up to date. */
    if (n > f_cache_max) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(f_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed */
        f_cache = tmp;
        for (idx_t i = f_cache_max + 1; i <= n; ++i) {
            f_cache[i] = f_cache[i - 1] + f_cache[i - 2];
        }
        f_cache_max = n;
    }

    /* Extend the a_cache to compute a(n). */
    if (n > a_cache_max) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed */
        a_cache = tmp;
        for (idx_t i = a_cache_max + 1; i <= n; ++i) {
            a_cache[i] = f_cache[i - 1] * a_cache[i - 1] + f_cache[i] * a_cache[i - 2];
        }
        a_cache_max = n;
    }

    return a_cache[n];
}
