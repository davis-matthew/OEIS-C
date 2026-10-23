/*
 * A129535  Number of permutations of 1,...,n with at least one pair of
 *          adjacent consecutive entries (i.e., of the form k(k+1) or (k+1)k),
 *          n >= 2.
 * https://oeis.org/A129535
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n-2)*a(n-1) - (n^2-2n-2)*a(n-2) + (n^2-7n+14)*a(n-3) *
 *            + (n-3)*(n-5)*a(n-4) - (n-3)*(n-4)*a(n-5) * Initial values:
 *            a(2)=2, a(3)=6, a(4)=22, a(5)=106
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Ensure cache large enough for indices up to n */
    if ((idx_t)(cache_size) <= n) {
        size_t new_size = (size_t)(n + 1);
        a_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        /* Initialize the new entries */
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            if (i == 2)
                a_cache[2] = (seq_t)2;
            else if (i == 3)
                a_cache[3] = (seq_t)6;
            else if (i == 4)
                a_cache[4] = (seq_t)22;
            else if (i == 5)
                a_cache[5] = (seq_t)106;
            else if (i >= 6) {
                /* Recurrence using only addition/subtraction of positive terms */
                seq_t t5 = (seq_t)(2 * i - 2) * a_cache[i - 1];
                seq_t t4 = (seq_t)(i * i - 7 * i + 14) * a_cache[i - 3];
                seq_t t3 = (seq_t)(i * i - 2 * i - 2) * a_cache[i - 2];
                seq_t t2 = (seq_t)(i - 3) * (i - 5) * a_cache[i - 4];
                seq_t t1 = (seq_t)(i - 3) * (i - 4) * a_cache[i - 5];

                /* a[i] = (t5 + t4 + t2) - (t3 + t1) */
                seq_t pos = t5;
                pos += t4;
                pos += t2;

                seq_t neg = t3;
                neg += t1;

                a_cache[i] = pos - neg;
            }
        }
        cache_size = new_size;
    }

    return a_cache[n];
}
