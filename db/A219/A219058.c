/*
 * A219058  Hilltop maps: number of nX3 binary arrays indicating the locations
 *          of corresponding elements not exceeded by any horizontal, diagonal
 *          or antidiagonal neighbor in a random 0..2 nX3 array.
 * https://oeis.org/A219058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) +7*a(n-2) +7*a(n-3) +3*a(n-4) +21*a(n-5)
 *            +6*a(n-6) +6*a(n-7) +3*a(n-8) +9*a(n-9) -9*a(n-10) -9*a(n-11)
 *            -9*a(n-12) -9*a(n-13) Initial values (n=1..13): 7, 63, 503,
 *            4017, 32123, 256849, 2053803, 16422459, 131315929, 1050017821,
 *            8396067527, 67135955941, 536827099717. The sequence is...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence type and index type are defined by the build. */
extern seq_t a(idx_t n)
{
    /* static cache of computed terms, index 1..cache_size */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* highest index stored */

    /* Initial 13 terms, 1-based indexing */
    static const seq_t init_terms[13] = {
        (seq_t)7,
        (seq_t)63,
        (seq_t)503,
        (seq_t)4017,
        (seq_t)32123,
        (seq_t)256849,
        (seq_t)2053803,
        (seq_t)16422459,
        (seq_t)131315929,
        (seq_t)1050017821,
        (seq_t)8396067527,
        (seq_t)67135955941,
        (seq_t)536827099717
    };

    /* Ensure cache holds at least the first 13 terms */
    if (cache_size < 13) {
        idx_t new_size = 13;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_size + 1));
        if (!new_cache)
            return (seq_t)0;   /* out of memory: will never happen in tests */
        cache = new_cache;
        for (idx_t i = 1; i <= 13; ++i)
            cache[i] = init_terms[i - 1];
        cache_size = new_size;
    }

    /* Return directly if within cached range */
    if (n <= cache_size)
        return cache[n];

    /* Coefficients of the recurrence (pos = positive, neg = negative) */
    static const int coeff[13] = {
        7,   /* a(n-1) */
        7,   /* a(n-2) */
        7,   /* a(n-3) */
        3,   /* a(n-4) */
        21,  /* a(n-5) */
        6,   /* a(n-6) */
        6,   /* a(n-7) */
        3,   /* a(n-8) */
        9,   /* a(n-9) */
       -9,   /* a(n-10) */
       -9,   /* a(n-11) */
       -9,   /* a(n-12) */
       -9    /* a(n-13) */
    };

    /* Extend cache to required size */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!new_cache)
        return (seq_t)0;   /* out of memory: will never happen in tests */
    cache = new_cache;

    /* Compute new terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int j = 0; j < 13; ++j) {
            seq_t val = cache[i - 1 - j];
            int c = coeff[j];
            if (c > 0) {
                sum_pos += (seq_t)c * val;
            } else { /* c < 0 */
                sum_neg += (seq_t)(-c) * val;
            }
        }
        cache[i] = sum_pos - sum_neg;  /* final result is positive   */
    }

    cache_size = n;
    return cache[n];
}
