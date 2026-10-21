/*
 * A067489  Powers of 3 with initial digit 1.
 * https://oeis.org/A067489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th power of 3 (3^k) that begins with the decimal digit 1.
 * The leading digit test uses the fractional part of k·log10(3):
 *  first digit 1  ⇔  frac(k·log10(3)) < log10(2).  */
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>          /* floor() on double values */

static const double LOG10_3 = 0.47712125471966244;   /* log10(3)  */
static const double LOG10_2 = 0.30102999566398114;   /* log10(2)  */

/* return true iff the power 3^exp starts with digit 1 */
static inline bool leading_digit_one(idx_t exp)
{
    double x   = (double)exp * LOG10_3;
    double frac = x - floor(x);          /* 0 ≤ frac < 1 */
    return frac < LOG10_2;
}

/* the requested sequence routine */
seq_t a(idx_t n)
{
    /* static state for incremental computation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static seq_t current     = 1;      /* 3^exp_current */
    static idx_t exp_current = 0;      /* exponent of current */
    static bool initialized  = false;

    if (!initialized) {
        cache_cap = 16;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failed */
        cache[0]   = (seq_t)1;         /* 3^0 */
        cache_len  = 1;
        exp_current = 1;
        current     = (seq_t)3;         /* 3^1 */
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        if (leading_digit_one(exp_current)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;   /* allocation failed */
                cache = tmp;
            }
            cache[cache_len++] = current;
        }
        /* advance to next power of 3 */
        current = current * (seq_t)3;     /* multiplication by 3 */
        exp_current++;
    }

    return cache[n - 1];
}
