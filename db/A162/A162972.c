/*
 * A162972  Number of cycles in all non-derangement permutations of
 *          {1,2,...,n}.
 * https://oeis.org/A162972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the D‑finite recurrence * a(n) + (-3*n+8)·a(n-1)
 *            + (n-3)*(3*n-14)·a(n-2) * + (-n³+19*n²-98*n+156)·a(n-3) * +
 *            (-4*n³+56*n²-258*n+395)·a(n-4) * +
 *            (-6*n³+84*n²-395*n+624)·a(n-5) * - (n-5)*(4*n²-41*n+106)·a(n-6)
 *            * - (n-5)*(n-6)²·a(n-7) = 0 * Initial values: a(1)=1, a(2)=2,
 *            a(3)=9, a(4)...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* static cache of computed terms */
static seq_t *cache = NULL;
static idx_t cached_n = 0;   /* number of terms stored (a(1)...a(n)) */

/* Helper: compute absolute value of a 64‑bit signed integer */
static inline idx_t abs_idx(idx_t x) { return x < 0 ? (idx_t)(-x) : (idx_t)x; }

seq_t a(idx_t n)
{
    /* ensure n >= 1; behaviour for n < 1 is undefined, return 0 */
    if (n == 0) return 0;

    /* increase cache size if needed */
    if (n > cached_n) {
        /* reallocate to hold new terms */
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed; abort (or return 0) */
            return 0;
        }
        cache = tmp;

        /* initial values a(1)...a(7) */
        static const seq_t init[7] = { (seq_t)1, (seq_t)2, (seq_t)9,
                                       (seq_t)38, (seq_t)210, (seq_t)1339,
                                       (seq_t)9870 };
        for (idx_t i = cached_n; i < n; ++i) {
            if (i < 7) {
                cache[i] = init[i];
                continue;
            }
            /* compute a(i+1) using recurrence with i+1 as n */
            idx_t N = i + 1;
            /* coefficients (all 64‑bit signed) */
            int64_t c0 = -3 * (int64_t)N + 8;
            int64_t c1 = (int64_t)(N - 3) * (3 * (int64_t)N - 14);
            int64_t c2 = - (int64_t)N * N * N + 19 * (int64_t)N * (int64_t)N
                         - 98 * (int64_t)N + 156;
            int64_t c3 = -4 * (int64_t)N * N * N + 56 * (int64_t)N * (int64_t)N
                         - 258 * (int64_t)N + 395;
            int64_t c4 = -6 * (int64_t)N * N * N + 84 * (int64_t)N * (int64_t)N
                         - 395 * (int64_t)N + 624;
            int64_t c5 = - (int64_t)(N - 5) *
                         (4 * (int64_t)N * (int64_t)N - 41 * (int64_t)N + 106);
            int64_t c6 = - (int64_t)(N - 5) *
                         (int64_t)(N - 6) * (int64_t)(N - 6);

            /* accumulate positive and negative contributions separately */
            seq_t pos = 0;
            seq_t neg = 0;
            /* coefficient 0 corresponds to a(n-1) */
            if (c0 > 0) pos += (seq_t)(c0) * cache[i - 1];
            else       neg += (seq_t)(-c0) * cache[i - 1];
            /* coefficient 1 -> a(n-2) */
            if (c1 > 0) pos += (seq_t)(c1) * cache[i - 2];
            else       neg += (seq_t)(-c1) * cache[i - 2];
            /* coefficient 2 -> a(n-3) */
            if (c2 > 0) pos += (seq_t)(c2) * cache[i - 3];
            else       neg += (seq_t)(-c2) * cache[i - 3];
            /* coefficient 3 -> a(n-4) */
            if (c3 > 0) pos += (seq_t)(c3) * cache[i - 4];
            else       neg += (seq_t)(-c3) * cache[i - 4];
            /* coefficient 4 -> a(n-5) */
            if (c4 > 0) pos += (seq_t)(c4) * cache[i - 5];
            else       neg += (seq_t)(-c4) * cache[i - 5];
            /* coefficient 5 -> a(n-6) */
            if (c5 > 0) pos += (seq_t)(c5) * cache[i - 6];
            else       neg += (seq_t)(-c5) * cache[i - 6];
            /* coefficient 6 -> a(n-7) */
            if (c6 > 0) pos += (seq_t)(c6) * cache[i - 7];
            else       neg += (seq_t)(-c6) * cache[i - 7];

            /* the recurrence gives: a(n) = neg - pos */
            cache[i] = neg - pos;
        }
        cached_n = n;
    }
    return cache[n - 1];
}
