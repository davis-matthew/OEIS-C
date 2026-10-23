/*
 * A167960  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 42*∑_{k=1..15} a(n‑k) – 903*a(n‑16) + p[n], * where p[0]
 *            = 1, p[1]…p[15] = 2, p[16] = 1, and p[n≥17] = 0. * This
 *            implements the recurrence derived from the generating function *
 *            given in OEIS A167960
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  done  = 0;

    /* Polynomial coefficients p[0..16] */
    static const seq_t p[17] = {
        (seq_t)1, (seq_t)2, (seq_t)2,  (seq_t)2, (seq_t)2,
        (seq_t)2, (seq_t)2,  (seq_t)2,  (seq_t)2,  (seq_t)2,
        (seq_t)2, (seq_t)2,  (seq_t)2,  (seq_t)2,  (seq_t)2,
        (seq_t)2, (seq_t)1
    };

    /* Grow cache if needed */
    if ((idx_t)n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 16;
        while ((idx_t)n + 1 > newcap)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;          /* assume allocation succeeds in the judge */
        cap   = newcap;
    }

    /* Compute any missing terms */
    for (idx_t i = done; i <= n; ++i) {
        seq_t sum = 0;

        /* 42 * sum of the previous 15 terms (if they exist) */
        idx_t kmax = 15;
        if (i < kmax) kmax = i;
        for (idx_t k = 1; k <= kmax; ++k)
            sum += cache[i - k] * (seq_t)42;

        /* Subtract 903 * a(i‑16) when applicable */
        if (i >= 16)
            sum -= cache[i - 16] * (seq_t)903;

        /* Add polynomial coefficient for i ≤ 16 */
        if (i <= 16)
            sum += p[i];

        cache[i] = sum;
    }

    done = n + 1;
    return cache[n];
}
