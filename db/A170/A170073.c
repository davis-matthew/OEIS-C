/*
 * A170073  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence derived from the rational generating function * * Let
 *            num(n) be the coefficient of t^n in the numerator: * num(0) = 1
 *            * num(1..36) = 2 * num(37) = 1 * num(n>=38) = 0 * * The
 *            denominator * 1 - 14 t - 14 t^2 - ... - 14 t^36 + 105 t^37 *
 *            yields the linear recurrence * * a(0) = 1 * for 1 ≤...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache = NULL;      /* cached sequence values   */
static idx_t   a_cache_len = 0;    /* number of valid entries   */
static idx_t   a_cache_cap = 0;    /* allocated capacity        */

seq_t a(idx_t n)
{
    /* Ensure cache exists and is large enough */
    if (a_cache == NULL) {
        a_cache_cap = 64;               /* initial capacity arbitrary small */
        a_cache = (seq_t*)malloc(a_cache_cap * sizeof(seq_t));
    }
    if (n >= a_cache_cap) {
        /* Grow cache to at least n+1 entries */
        idx_t new_cap = a_cache_cap ? a_cache_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = a_cache_len; i <= n; ++i) {
        seq_t num_val;
        if (i == 0)          num_val = 1;
        else if (i <= 36)    num_val = 2;
        else if (i == 37)    num_val = 1;
        else                 num_val = 0;

        /* Sum of previous 36 terms (a[i-1] ... a[i-36]) */
        seq_t sum36 = 0;
        for (idx_t j = 1; j <= 36 && j <= i; ++j) {
            sum36 += a_cache[i - j];
        }

        /* Initialise with num_val and add 14*sum36 */
        seq_t val = num_val;
        val += (seq_t)14 * sum36;

        /* Subtract 105*a[i-37] when applicable (i >= 37) */
        if (i >= 37) {
            val -= (seq_t)105 * a_cache[i - 37];
        }

        a_cache[i] = val;
    }
    a_cache_len = n + 1;

    return a_cache[n];
}
