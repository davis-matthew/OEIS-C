/*
 * A170340  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function of A170340.
   The G.f. is
          (t^42 + 2*t^41 + ... + 2*t + 1) /
   ( 1 - 41*t - 41*t^2 - ... - 41*t^41 + 861*t^42 )
   Let a(0)=1.  For n ≥ 0 the coefficients satisfy
        a(n) = 41 * Σ_{k=1}^{min(41,n)} a(n-k)
               - 861 * a(n-42) + p_n
   where p_n = 1 if n==0 or n==42,
             = 2 if 1 ≤ n ≤ 41,
             = 0 otherwise.
   This recurrence produces the sequence:
   a(0)=1, a(1)=43, a(2)=1806, a(3)=75852, … .
   The function memoises values so that successive calls are O(1) amortised.
*/

#include <stdlib.h>

static seq_t *cache = NULL;     /* memoisation buffer */
static idx_t cache_len = 0;     /* number of terms stored (next free index) */
static size_t cache_cap = 0;    /* current capacity of 'cache' */

/* Ensure that the cache can hold at least 'need' elements. */
static void ensure_capacity(idx_t need) {
    if (need <= cache_cap) return;

    /* grow by a factor of two, but not less than required size */
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < (size_t)need) new_cap = (size_t)need;

    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In a real judge environment, one would check for NULL.  */
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    /* grow cache up to index 'n' if needed */
    if (cache_len <= n) {
        ensure_capacity(n + 1);
        for (idx_t i = cache_len; i <= n; ++i) {
            /* compute sum of the previous up to 41 terms */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 41 && k <= i; ++k) {
                sum += cache[i - k];
            }
            /* term = 41 * sum */
            seq_t term = sum * 41;

            /* subtract 861 * a(i-42) if i >= 42 */
            if (i >= 42) {
                term -= cache[i - 42] * 861;
            }

            /* add p_i term */
            if (i == 0 || i == 42) {
                term += 1;
            } else if (i <= 41) {
                term += 2;
            }

            cache[i] = term;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
