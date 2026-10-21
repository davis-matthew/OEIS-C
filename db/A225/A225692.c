/*
 * A225692  Number of Dyck paths of semilength n avoiding the pattern U^(n-1)
 *          D^(n-1).
 * https://oeis.org/A225692
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by * * -10*(n+1)*(n-4)*a(n) +
 *            (51*n^2-196*n+80)*a(n-1) * + (-45*n^2+134*n-99)*a(n-2) +
 *            2*(2*n-5)*(n-3)*a(n-3) = 0 * * with initial values a(2)=0,
 *            a(3)=0, a(4)=4. For n >= 5: * * a(n) = [
 *            (51*n^2-196*n+80)*a(n-1) * + (-45*n^2+134*n-99)*a(n-2) * +
 *            2*(2*n-5)*(n-3)*a(n-3) ] / (10*(n+1)*(...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Below the offset (n < 2) is irrelevant. */
    if (n < 2) return (seq_t)0;

    /* Static memoisation table. */
    static seq_t *cache = NULL;   /* array of a(k) for 0 <= k <= cached */
    static idx_t cached = 0;      /* highest computed index */
    static size_t cap = 0;        /* current allocation size (abs index + 1) */

    /* First call: allocate minimal space and initialise known values. */
    if (cache == NULL) {
        cap = (size_t)n + 1;
        if (cap < 5) cap = 5;                /* need space up to index 4 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* initialise unused lower indices and first terms */
        cache[0] = cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 4;
        cached = 4;
    }

    /* Grow the array if necessary.  Copy existing values. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        memcpy(new_cache, cache, sizeof(seq_t) * (cached + 1));
        free(cache);
        cache = new_cache;
        cap = new_cap;
    }

    /* Return the value if already computed. */
    if (n <= cached) return cache[n];

    /* Compute missing terms up to n. */
    for (idx_t k = cached + 1; k <= n; ++k) {
        if (k == 2 || k == 3) {
            cache[k] = 0;
        } else if (k == 4) {
            cache[k] = 4;
        } else {
            /* Coefficients: all positive except the subtraction term.
               We keep the subtraction separate to avoid negative numbers. */
            idx_t kk = k;
            seq_t coeff1 = (seq_t)((idx_t)51 * kk * kk
                                   - (idx_t)196 * kk
                                   + (idx_t)80);
            seq_t coeff2 = (seq_t)((idx_t)45 * kk * kk
                                   - (idx_t)134 * kk
                                   + (idx_t)99); /* used with subtraction */
            seq_t coeff3 = (seq_t)2 * (seq_t)(2 * kk - 5) * (seq_t)(kk - 3);

            /* Numerator = coeff1*a(k-1) + coeff3*a(k-3) - coeff2*a(k-2) */
            seq_t num = cache[k-1];
            num *= coeff1;

            seq_t tmp = cache[k-3];
            tmp *= coeff3;
            num += tmp;

            seq_t sub = cache[k-2];
            sub *= coeff2;
            num -= sub;          /* guaranteed non‑negative for all k >= 5 */

            /* Denominator = 10*(k+1)*(k-4) */
            seq_t denom = (seq_t)10 * (seq_t)(k + 1) * (seq_t)(k - 4);

            cache[k] = num / denom;    /* exact division */
        }
    }
    cached = n;
    return cache[n];
}
