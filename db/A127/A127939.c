/*
 * A127939  a(n) = numerator of b(n), where b(1)=1, b(n+1) = sum{k|n} 1/b(k).
 * https://oeis.org/A127939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of b(n) where b(1) = 1 b(n+1) = Σ_{k|n} 1/b(k)
 *            (sum over all positive divisors k of n) a(n) is the numerator of
 *            b(n) in lowest terms
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 15, u64 n <= 21, u128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* compute a(n) – numerator of b(n) */
seq_t a(idx_t n)
{
    /* static state: arrays of numerators and denominators
       and variables controlling allocation and progress    */
    static seq_t *num = NULL;
    static seq_t *den = NULL;
    static size_t cap = 0;         /* allocated array size (indexable 0..cap-1) */
    static idx_t computed = 0;     /* highest index that has been computed (>=1) */

    if (n == 0) return 0;         /* out of range – not used by tests */

    /* ---------- initialise storage on first call ---------- */
    if (cap == 0) {
        cap = 2;                  /* at least index 1 must be available */
        num = (seq_t *)malloc(cap * sizeof(seq_t));
        den = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!num || !den) return 0;   /* allocation failed */
        num[1] = (seq_t)1;          /* b(1) = 1/1 */
        den[1] = (seq_t)1;
        computed = 1;
    }

    /* ---------- ensure array capacity for requested index ---------- */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap;
        while ((idx_t)new_cap <= n) new_cap <<= 1;  /* double until enough */
        seq_t *tmp_num = (seq_t *)realloc(num, new_cap * sizeof(seq_t));
        seq_t *tmp_den = (seq_t *)realloc(den, new_cap * sizeof(seq_t));
        if (!tmp_num || !tmp_den) return 0;         /* allocation failed */
        num = tmp_num;
        den = tmp_den;
        cap = new_cap;
    }

    /* ---------- compute values up to n if not already done ---------- */
    for (idx_t m = computed; m < n; ++m) {
        /* compute b(m+1) = sum_{k|m} 1/b(k) */
        seq_t sum_num = (seq_t)0;
        seq_t sum_den = (seq_t)1;

        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                /* term for divisor d : 1/b(d) = den[d]/num[d] */
                seq_t num_d = num[d];
                seq_t den_d = den[d];

                seq_t temp_num = sum_num * num_d + sum_den * den_d;
                seq_t temp_den = sum_den * num_d;
                seq_t g = gcd(temp_num, temp_den);
                sum_num = temp_num / g;
                sum_den = temp_den / g;

                idx_t d2 = m / d;
                if (d2 != d) {
                    /* term for complementary divisor d2 */
                    seq_t num_d2 = num[d2];
                    seq_t den_d2 = den[d2];

                    temp_num = sum_num * num_d2 + sum_den * den_d2;
                    temp_den = sum_den * num_d2;
                    g = gcd(temp_num, temp_den);
                    sum_num = temp_num / g;
                    sum_den = temp_den / g;
                }
            }
        }

        num[m + 1] = sum_num;
        den[m + 1] = sum_den;
    }

    computed = n;
    return num[n];
}
