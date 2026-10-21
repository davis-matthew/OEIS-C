/*
 * A220635  Number of ways to reciprocally link elements of an 5Xn array
 *          either to themselves or to exactly two horizontal or antidiagonal
 *          neighbors.
 * https://oeis.org/A220635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the linear recurrence a(n) = 16*a(n-1) - 39*a(n-2)
 *            - 186*a(n-3) + 419*a(n-4) +1024*a(n-5) -1049*a(n-6) -2258*a(n-7)
 *            +861*a(n-8) +2000*a(n-9) -133*a(n-10) -586*a(n-11) -16*a(n-12)
 *            +32*a(n-13), for n > 17 with initial values a(1)=1, a(2)=1,
 *            a(3)=8, a(4)=114, a(5)=1481, a(6)=17527, a(7)...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;

    /* initial values for n = 1 .. 17 (stored 0‑based) */
    static const seq_t init_vals[17] = {
        1, 1, 8, 114, 1481, 17527, 201903, 2329448,
        26904225, 310837917, 3591561452, 41499180881ULL,
        479508472309ULL, 5540546671074ULL, 64018957168901ULL,
        739714921940047ULL, 8547125318062702ULL
    };

    /* enlarge cache if necessary */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 17;
        if (n > new_cap) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ensure the first 17 values are stored */
    if (last_computed < 17) {
        for (size_t i = 0; i < 17; ++i)
            cache[i] = init_vals[i];
        last_computed = 17;
    }

    /* return immediately if already computed */
    if (n <= last_computed) return cache[n-1];

    /* coefficients and their offsets for the recurrence */
    static const int pos_offsets[6] = {1, 4, 5, 8, 9, 13};
    static const seq_t pos_coeffs[6] = {16, 419, 1024, 861, 2000, 32};

    static const int neg_offsets[7] = {2, 3, 6, 7, 10, 11, 12};
    static const seq_t neg_coeffs[7] = {39, 186, 1049, 2258, 133, 586, 16};

    /* compute values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        idx_t idx = i - 1;               /* 0‑based index for a(i) */

        seq_t sum_pos = 0;
        for (int j = 0; j < 6; ++j)
            sum_pos += pos_coeffs[j] * cache[idx - pos_offsets[j]];

        seq_t sum_neg = 0;
        for (int j = 0; j < 7; ++j)
            sum_neg += neg_coeffs[j] * cache[idx - neg_offsets[j]];

        cache[idx] = sum_pos - sum_neg;
    }
    last_computed = n;
    return cache[n-1];
}
