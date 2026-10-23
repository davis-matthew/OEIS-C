/*
 * A167112  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1..13) = 11 * 10^(n-1) a(14) = 11 * 10^13 - 55 a(n) =
 *            9 * (a(n-1) + a(n-2) + … + a(n-13)) – 45 * a(n-14), for n >= 15
 *            (Recurrence derived from the rational generating function. The
 *            special values for n <= 14 are directly from the initial terms.)
 *            The function memoizes computed values; i...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   mul_small, pow10, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#include "../utils/pow10.h"     /* seq_t pow10(idx_t exp);          */
#include "../utils/mul_small.h" /* seq_t mul_small(seq_t x, unsigned int m); */
#include "../utils/seq_from_str.h" /* seq_t seq_from_str(const char *s);       */

static seq_t *a_cache   = NULL;   /* memoization array   */
static idx_t  a_max     = (idx_t)-1;/* highest index cached, -1 = none yet */
static size_t a_cap     = 0;        /* capacity of a_cache */

/* Ensure a_cache has room for indices 0 … n inclusive. */
static void grow_cache(idx_t n)
{
    if (a_cap <= n) {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_ptr = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();                /* memory exhausted */
        a_cache = new_ptr;
        a_cap   = new_cap;
    }
}

/* Compute a(n) and store it in a_cache[n] (assumes indices < n already computed). */
static void compute_value(idx_t n)
{
    if (n == 0) {
        a_cache[0] = 1;
    } else if (n == 1) {
        a_cache[1] = 11;
    } else if (n >= 2 && n <= 13) {
        seq_t p10 = pow10(n - 1);          /* 10^(n-1) */
        a_cache[n] = mul_small(p10, 11);    /* 11 * 10^(n-1) */
    } else if (n == 14) {
        seq_t p10 = pow10(13);             /* 10^13 */
        seq_t val = mul_small(p10, 11);     /* 11 * 10^13 */
        seq_t sub = 55;                    /* 55 */
        a_cache[14] = val - sub;
    } else { /* n >= 15 */
        seq_t sum = 0;
        for (idx_t k = 1; k <= 13; ++k) {
            sum += a_cache[n - k];
        }
        sum = mul_small(sum, 9);            /* 9 * sum_{i=1}^{13} a(n-i) */
        seq_t subtract = mul_small(a_cache[n - 14], 45); /* 45 * a(n-14) */
        a_cache[n] = sum - subtract;
    }
}

/* Ensure that all values up to index n are computed. */
static void ensure_computed(idx_t n)
{
    if (a_max != (idx_t)-1 && a_max >= n) return;   /* already done */

    grow_cache(n);

    if (a_max == (idx_t)-1) {                       /* first time */
        for (idx_t i = 0; i <= n; ++i) compute_value(i);
        a_max = n;
    } else {
        for (idx_t i = a_max + 1; i <= n; ++i) compute_value(i);
        a_max = n;
    }
}

seq_t
a(idx_t n)
{
    ensure_computed(n);
    return a_cache[n];
}
