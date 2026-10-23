/*
 * A167094  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A167094: Number of reduced words of length n in a Coxeter group with
   41 generators S_i, relations S_i^2=I and (S_i S_j)^13=I
   Generating function:
        (t^13 + 2t^12 + 2t^11 + 2t^10 + 2t^9 + 2t^8 + 2t^7 + 2t^6 +
         2t^5 + 2t^4 + 2t^3 + 2t^2 + 2t + 1)
        -----------------------------------------------------
        (780t^13 - 39t^12 - 39t^11 - 39t^10 - 39t^9 - 39t^8 -
         39t^7 - 39t^6 - 39t^5 - 39t^4 - 39t^3 - 39t^2 - 39t + 1)

   The coefficients satisfy the linear recurrence derived from
   (1 - 39t - 39t^2 - … - 39t^12 + 780t^13) * G(t) = P(t)
   where P(t) is the numerator above.
   Let a[n] denote the n‑th term.  For n ≥ 14:
        a[n] = 39 * Σ_{k=1}^{12} a[n-k] - 780 * a[n-13].
   For 0 ≤ n ≤ 12:
        a[n] = p[n] + 39 * Σ_{k=1}^{n} a[n-k]          (p[n] = 1 if n = 0 or 13,
                                                      p[n] = 2 if 1 ≤ n ≤ 12,
                                                      p[n] = 0 otherwise).
   For n = 13:
        a[13] = 1 + 39 * Σ_{k=1}^{12} a[13-k] - 780 * a[0].

   This implementation memoises the values of a[n] in a static
   array so that multiple calls, in any order, are efficient.  The
   recurrence is computed using only integer arithmetic-without
   floating‑point.  The code works for any build where seq_t and
   idx_t are integer types, including arbitrary‑precision integer
   classes.  Complexity is linear in the maximum n requested. */

#include <stdlib.h>

static seq_t *cache   = NULL;    /* array of computed a[n] values  */
static size_t cache_size = 0u;   /* number of filled entries in cache */

static void ensure_cache(idx_t n)
{
    if (cache == NULL || cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        if (cache == NULL) {
            abort(); /* allocation failure */
        }
        /* initialise the first 14 terms if needed */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* compute coefficient p[i] of the numerator P(t) */
            seq_t p = 0;
            if (i == 0 || i == 13) {
                p = 1;
            } else if (1 <= i && i <= 12) {
                p = 2;
            } else {
                p = 0;
            }

            /* compute a[i] based on recurrence */
            seq_t sum = 0;
            /* sum over k = 1..12 (but only existing terms) */
            idx_t limit = i > 12 ? 12 : i;
            for (idx_t k = 1; k <= limit; ++k) {
                sum += cache[(size_t)(i - k)];
            }

            seq_t val = sum * (seq_t)39;  /* 39 * sum */

            if (i < 13) {          /* 0 ≤ i ≤ 12 */
                val += p;          /* a[i] = p + 39*sum */
            } else if (i == 13) {  /* special case for 13 */
                val += p;          /* p is 1 here */
                val -= (seq_t)780 * cache[0];   /* subtract 780*a[0] */
            } else {              /* i ≥ 14 */
                val -= (seq_t)780 * cache[(size_t)(i - 13)];
            }

            cache[(size_t)i] = val;
        }
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
