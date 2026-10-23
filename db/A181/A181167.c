/*
 * A181167  G.f.: 1 = Sum_{n>=0} a(n)*x^n* Sum_{k>=0} C(2n+k,k)^2*(-x)^k.
 * https://oeis.org/A181167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} (-1)^{k+1} * a(n-k) * C(2n-k, k)^2 , a(0)=1
 *            The recurrence is derived from the identity 1 = ∑_{i≥0} a(i)x^i
 *            * ∑_{k≥0} C(2i+k, k)^2 (-x)^k. This implementation memoises
 *            computed values in a static dynamic array and evaluates the
 *            binomial factors incrementally for each n. The algo...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cap = 0;    /* allocated size */
    static idx_t last = 0;    /* highest n whose value is stored */

    /* enlarge cache if needed */
    if (n >= cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* out‑of‑memory */
        cache = tmp;
        cap = new_cap;
    }

    /* initialise a(0) once */
    if (last == 0 && cap > 0) {
        cache[0] = (seq_t)1;
    }

    /* compute new terms up to n */
    if (n > last) {
        for (idx_t m = last + 1; m <= n; ++m) {
            seq_t pos = 0, neg = 0;
            /* B_k = C(2m - k, k), start with k = 1 */
            seq_t B = (seq_t)(2 * m - 1);   /* C(2m-1,1) */

            for (idx_t k = 1; k <= m; ++k) {
                /* term = a(m-k) * B^2 */
                seq_t B2 = B * B;
                seq_t term = B2 * cache[m - k];

                /* add with appropriate sign: k=1 -> +, k=2 -> -, ... */
                if (k & 1) {
                    pos += term;
                } else {
                    neg += term;
                }

                /* update B to B_{k+1} if needed */
                if (k < m) {
                    /* B = B * (2m - 2k) * (2m - 2k - 1) / ((k+1) * (2m - k)) */
                    seq_t num1 = (seq_t)(2 * m - 2 * k);
                    seq_t num2 = (seq_t)(2 * m - 2 * k - 1);
                    seq_t den  = (seq_t)((k + 1) * (2 * m - k));

                    B = B * num1;      /* multiply by num1 */
                    B = B * num2;      /* multiply by num2 */
                    B = B / den;       /* divide by den (exact) */
                }
            }

            cache[m] = pos - neg;   /* pos ≥ neg for all m ≥ 1 */
        }
        last = n;
    }

    return cache[n];
}
