/*
 * A229737  G.f.: x^2*(1-x)*(1-4*x+4*x^2+x^3)/((1-2*x)^2*(1-3*x+x^2)).
 * https://oeis.org/A229737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A229737 – Recurrence form
   G.f.: x^2*(1-x)*(1-4*x+4*x^2+x^3)/((1-2*x)^2*(1-3*x+x^2))
   Recurrence (derived from the denominator):
     a(0) = 0, a(1) = 0
     a(n) = 7*a(n-1) – 17*a(n-2) + 16*a(n-3) – 4*a(n-4) + p(n)
   where the inhomogeneous part p(n) comes from the numerator:
     p(2)= 1, p(3)= –5, p(4)= 8, p(5)= –3, p(6)= –1,  p(n)=0 otherwise.
   The recurrence is valid for all n ≥ 0 (interpreting a(k)=0 for k<0). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoised values */
    static idx_t cache_size = 0;     /* number of cached terms */

    /* Ensure the cache can hold n+1 terms (indices 0..n). */
    if ((idx_t)(n + 1) > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 as a safe failure value. */
            return 0;
        }
        cache = tmp;

        /* Compute missing terms. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0 || i == 1) {
                cache[i] = 0;
                continue;
            }

            /* Positive contributions: 7*a(i-1) + 16*a(i-3) + p(i). */
            seq_t pos = 0;
            if (i >= 1) pos += 7 * cache[i - 1];
            if (i >= 3) pos += 16 * cache[i - 3];

            /* Inhomogeneous term p(i). */
            int p = 0;
            switch (i) {
                case 2: p =  1; break;
                case 3: p = -5; break;
                case 4: p =  8; break;
                case 5: p = -3; break;
                case 6: p = -1; break;
                default: p = 0; break;
            }
            if (p >= 0)
                pos += (seq_t)p;
            else
                pos -= (seq_t)(-p);

            /* Negative contributions: 17*a(i-2) + 4*a(i-4). */
            seq_t neg = 0;
            if (i >= 2) neg += 17 * cache[i - 2];
            if (i >= 4) neg += 4 * cache[i - 4];

            /* a(i) = pos - neg (guaranteed non‑negative). */
            cache[i] = pos - neg;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
