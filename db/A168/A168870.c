/*
 * A168870  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 47*(a(n-1)+a(n-2)+…+a(n-19)) - 1128*a(n-20) (n≥20)
 *            Initial values for 0≤n≤20 are obtained from the numerator of the
 *            generating function: b(0)=1, b(1..19)=2, b(20)=1. For n>20
 *            coefficients from the numerator are zero. The recurrence is used
 *            successively to compute all terms. The sequence is...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdint.h>   /* for size_t */

static seq_t *cache = NULL;          /* cache of computed a(n) values   */
static idx_t cache_size = 0;         /* number of terms stored   */
static size_t cache_cap = 0;         /* current capacity of cache */

/* Ensure space for at least (need + 1) elements.  Reallocates with a
   new larger array and copies existing values.  This works for C and
   C++ when seq_t is trivially copyable (as expected for the build
   types). */
static void ensure_capacity(idx_t need)
{
    if ((size_t)need + 1 > cache_cap) {
        size_t new_cap = ((size_t)need + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute a(i) for all i from cache_size up to n (inclusive).  The
   recurrence derived from the rational generating function is used:
     a(i) = b_i + 47*a(i-1)+…+47*a(i-19) − 1128*a(i-20)
   where b_i is the coefficient of t^i in the numerator; b_i=1 if
   i==0 or i==20, b_i=2 if 1≤i≤19, and b_i=0 otherwise. */
static void compute_up_to(idx_t n)
{
    ensure_capacity(n);

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t value = 0;
        /* numerator contribution b_i */
        if (i == 0 || i == 20) {
            value = (seq_t)1;
        } else if (i >= 1 && i <= 19) {
            value = (seq_t)2;
        } else {
            value = (seq_t)0;
        }

        /* add 47 * a(i-j) for j = 1..19 if available */
        for (int j = 1; j <= 19; ++j) {
            if (i >= (idx_t)j) {
                /* (seq_t)47 is an integer constant cast to seq_t */
                value += (seq_t)47 * cache[i - j];
            }
        }

        /* subtract 1128 * a(i-20) if available */
        if (i >= 20) {
            value -= (seq_t)1128 * cache[i - 20];
        }

        cache[i] = value;
    }
    cache_size = n + 1;   /* now terms 0 … n are stored */
}

/* Public function requested by the specification. */
seq_t a(idx_t n)
{
    if (n >= cache_size) {
        compute_up_to(n);
    }
    return cache[n];
}
