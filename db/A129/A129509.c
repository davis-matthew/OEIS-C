/*
 * A129509  G.f.: (1+x+x^2-sqrt(1+2x+3x^2-2x^3+x^4))/2.
 * https://oeis.org/A129509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) satisfies n*a(n) + (2*n-3)*a(n-1) + 3*(n-3)*a(n-2) +
 *            (9-2*n)*a(n-3) + (n-6)*a(n-4) = 0 for n>5. Initial values:
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=-1, a(5)=0
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 33, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A129509. */
seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for 0 <= i < cached. */
    static seq_t *cache = NULL;
    static size_t cached = 0;   /* number of cached terms (cached = max index + 1) */

    /* Grow cache if needed.   */
    if (n >= cached) {
        size_t new_cap = n + 1;
        if (cache == NULL) {
            cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
            if (!cache) exit(EXIT_FAILURE);
        } else {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
        }

        /* Fill new entries.   */
        for (idx_t i = cached; i <= n; ++i) {
            if (i == 0 || i == 1 || i == 2) {
                cache[i] = 0;
            } else if (i == 3) {
                cache[i] = 1;
            } else if (i == 4) {
                cache[i] = -1;
            } else if (i == 5) {
                cache[i] = 0;
            } else { /* recurrence for i > 5 */
                seq_t a_nm1 = cache[i-1];
                seq_t a_nm2 = cache[i-2];
                seq_t a_nm3 = cache[i-3];
                seq_t a_nm4 = cache[i-4];

                seq_t term1 = (seq_t)((2LL * (long long)i) - 3LL) * a_nm1;
                seq_t term2 = (seq_t)(3LL * ((long long)i - 3LL)) * a_nm2;
                seq_t term3 = (seq_t)(9LL - 2LL * (long long)i) * a_nm3;
                seq_t term4 = (seq_t)((long long)i - 6LL) * a_nm4;

                seq_t numerator = -(term1 + term2 + term3 + term4);
                cache[i] = numerator / (seq_t)i;
            }
        }
        cached = new_cap;
    }
    return cache[n];
}
