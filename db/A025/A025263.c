/*
 * A025263  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+ a(n-1)*a(1) for n >= 4,
 *          with initial terms 1,2,1.
 * https://oeis.org/A025263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)*a(n-1)+a(2)*a(n-2)+...+a(n-1)*a(1) for n ≥ 4 Initial
 *            terms: a(1)=1, a(2)=2, a(3)=1 Linear recurrence: n*a(n) =
 *            2*(2*n-3)*a(n-1) + 4*(n-3)*a(n-2) - 6*(2*n-9)*a(n-3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms, 1‑based */
    static idx_t cap   = 0;       /* largest index allocated      */
    static idx_t last  = 0;       /* largest index computed       */

    /* Handle the first three terms directly */
    if (n <= 3) {
        return (n == 1) ? (seq_t)1 :
               (n == 2) ? (seq_t)2 : (seq_t)1;
    }

    /* First call – allocate space for indices 0…3 (the 0th element is unused) */
    if (!cache) {
        cap = 4;                                   /* at least indices 0..3 */
        cache = (seq_t*)malloc(sizeof(seq_t) * (cap + 1));
        if (!cache) abort();                       /* OOM protection */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
        last    = 3;
    }

    /* Extend the cache when the requested n exceeds the current capacity */
    if (n > cap) {
        idx_t new_cap = n;                         /* need indices 0…n      */
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (!tmp) abort();                         /* OOM protection */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing values up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sn = (seq_t)i;

        seq_t term1 = (seq_t)2 * ((seq_t)2 * sn - (seq_t)3) * cache[i-1];
        seq_t term2 = (seq_t)4 * (sn - (seq_t)3) * cache[i-2];
        seq_t term3 = (seq_t)6 * ((seq_t)2 * sn - (seq_t)9) * cache[i-3];

        seq_t num = term1 + term2 - term3;         /* numerator of the formula */
        cache[i]  = num / sn;                     /* division is exact */
    }

    last = n;
    return cache[n];
}
