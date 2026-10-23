/*
 * A143567  E.g.f. satisfies A(x) = exp(x*A(x^3/3!)).
 * https://oeis.org/A143567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=(n-1)! * Σ_{k=0..⌊(n-1)/3⌋} (3k+1) * a(k) *
 *            a(n-1-3k) / (6^k * k! * (n-1-3k)! )
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 24, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Memoisation cache */
static seq_t *cache      = NULL;   /* array of computed terms          */
static idx_t  cache_cap  = 0;      /* capacity of cache (in terms)     */
static idx_t  last_comp  = 0;      /* highest index that has been computed */

/* Grow cache and initialise base case */
static void ensure_cache(idx_t n)
{
    /* Allocate (at least) space up to index n */
    if (n >= cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();             /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Ensure a(0)=1 is set */
    if (n == 0) {
        cache[0] = 1;
        last_comp = 0;
        return;
    }
    if (cache[0] == 0) cache[0] = 1;   /* initialise if not yet */

    /* Compute missing terms up to n */
    if (n <= last_comp) return;        /* nothing to do */

    for (idx_t i = last_comp + 1; i <= n; ++i) {
        /* Recurrence for a(i) (i >= 1) */
        idx_t m = i - 1;               /* m = n-1 */
        seq_t sum   = 0;
        seq_t factor = 1;              /* factor for k=0 */

        for (idx_t k = 0; k <= m / 3; ++k) {
            idx_t km = m - 3 * k;                     /* index for a(i-1-3k) */
            seq_t term = factor;
            term *= (seq_t)(3 * k + 1);
            term *= cache[k];
            term *= cache[km];
            sum += term;

            /* Prepare factor for next k, if any */
            if (k < m / 3) {
                seq_t num1 = (seq_t)(m - 3 * k);      /* m - 3k          */
                seq_t num2 = num1 - 1;                /* m - 3k - 1      */
                seq_t num3 = num2 - 1;                /* m - 3k - 2      */
                seq_t nxt   = factor;
                nxt *= num1;
                nxt *= num2;
                nxt *= num3;
                nxt /= (seq_t)(k + 1);                 /* divide by (k+1)  */
                nxt /= 6;                               /* divide by 6      */
                factor = nxt;
            }
        }
        cache[i] = sum;
    }
    last_comp = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
