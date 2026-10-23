/*
 * A170569  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1 For 1 ≤ n ≤ 46: a(n)=2 + 30*∑_{k=1}^{46} a(n-k) For n=47:
 *            a(47)=1 + 30*∑_{k=1}^{46} a(47-k) - 465*a(0) For n≥48: a(n)=p[n]
 *            + 30*∑_{k=1}^{46} a(n-k) - 465*a(n-47) where p[n]=1 if n=0 or
 *            n=47, p[n]=2 if 1≤n≤46, and p[n]=0 otherwise. This recurrence
 *            comes from the rational generating function...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms */
static idx_t cache_size = 0;      /* number of terms currently stored */

seq_t a(idx_t n) {
    /* Allocate or grow the cache if necessary */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure -> return 0 */
        cache = tmp;

        /* Fill terms from the current cache_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t p = 0;
            if (i == 0)                  p = 1;
            else if (i <= 46)             p = 2;
            else if (i == 47)             p = 1;
            /* otherwise p remains 0 */

            /* Sum of the previous 46 terms */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 46; ++k) {
                if (i >= k) sum += cache[i - k];
            }

            /* Compute a[i] = p + 30*sum - 465*a[i-47]  (if i>=47) */
            seq_t val = sum;
            val *= 30;            /* val = 30*sum */
            val += p;             /* val += p */
            if (i >= 47) {
                seq_t sub = cache[i - 47];
                sub *= 465;       /* sub = 465*a[i-47] */
                val -= sub;       /* val -= sub */
            }
            cache[i] = val;
        }
        cache_size = new_size;
    }
    return cache[n];
}
