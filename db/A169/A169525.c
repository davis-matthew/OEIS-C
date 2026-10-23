/*
 * A169525  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence derived from the generating
 *            function F(t) = (t^34 + 2*t^33 + … + 2*t + 1) / (1 - 30*t -
 *            30*t^2 - … - 30*t^33 + 465*t^34). For n ≥ 0: a(n) = b(n) + 30 *
 *            Σ_{k=1}^{min(n,33)} a(n-k) – 465 * a(n-34), where b(0)=1,
 *            b(1..33)=2, b(34)=1, b(n)=0 for n>34
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* stored values */
    static size_t cap   = 0;        /* allocated capacity */
    static idx_t  len   = 0;        /* number of values stored (next free index) */

    /* expand cache if needed */
    if (n >= len) {
        size_t new_cap = cap;
        if (new_cap < 8) new_cap = 8;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;

        if (new_cap > cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
            if (!tmp) return (seq_t)0;     /* memory error */
            cache = tmp;
            cap   = new_cap;
        }

        /* compute terms up to n */
        for (idx_t i = len; i <= n; ++i) {
            /* RHS coefficient of t^i in N(t) */
            seq_t rhs;
            if (i == 0 || i == 34)
                rhs = (seq_t)1;
            else if (i <= 33)
                rhs = (seq_t)2;
            else
                rhs = (seq_t)0;   /* never used for i > 34 */

            /* sum of the previous up to 33 terms */
            seq_t sumprev = (seq_t)0;
            idx_t limit = (i < 33) ? i : 33;
            for (idx_t k = 1; k <= limit; ++k)
                sumprev += cache[i - k];

            seq_t val = rhs + (seq_t)30 * sumprev;
            if (i >= 34)
                val -= (seq_t)465 * cache[i - 34];

            cache[i] = val;
        }
        len = n + 1;
    }
    return cache[n];
}
