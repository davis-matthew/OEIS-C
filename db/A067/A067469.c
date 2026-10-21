/*
 * A067469  Numbers k such that 2 is the first digit of 2^k.
 * https://oeis.org/A067469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) returns the nth integer k such that the first digit of 2^k
 *            is 2. * The computation uses the fractional part of k·log10(2)
 * Range:     u8 n <= 47, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* Constant log10(2) as a long double for accuracy. */
static const long double LOG10_2 = 0.30102999566398119521373889472449302L;

/* Test whether the leading digit of 2^k is 2. */
static bool leading_digit_is_two(idx_t k)
{
    long double t    = (long double)k * LOG10_2;
    long double frac = t - floorl(t);          /* fractional part of k·log10(2) */
    long double y    = powl(10.0L, frac);      /* 10^frac gives the leading digits fraction */
    /* Adding a tiny epsilon avoids floating-point rounding making floor slightly
       smaller than the true value. */
    return floorl(y + 1e-12L) == 2.0L;
}

/* Static cache for already computed a(n) values. */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_n   = 0;
static idx_t  next_k    = 1;          /* next exponent to check for leading 2 */

static void ensure_cache_capacity(void)
{
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined for offset <1; harmless. */

    /* Generate terms until we have at least n entries. */
    while (cache_n < n) {
        if (leading_digit_is_two(next_k)) {
            ensure_cache_capacity();
            cache[cache_n++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
