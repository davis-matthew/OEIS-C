/*
 * A234424  Number of (n+1)X(1+1) 0..7 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to 14
 *          and no adjacent elements equal.
 * https://oeis.org/A234424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) +64*a(n-2) -451*a(n-3) -1542*a(n-4) +7659*a(n-5)
 *            +16809*a(n-6) -48643*a(n-7) -76610*a(n-8) +75468*a(n-9)
 *            +91816*a(n-10) -33928*a(n-11) -30960*a(n-12); Initial values
 *            (n=1..12): a(1) = 448 a(2) = 4516 a(3) = 44140 a(4) = 453728
 *            a(5) = 4641220 a(6) = 47946684 a(7) = 494928936 a(8) =...
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed = 0;      /* highest n already computed */

    if (n < 1) return (seq_t)0;     /* sequence defined for n ≥ 1 */

    /* Initial constants for the first 12 terms */
    static const seq_t init[13] = {
        0ULL, /* unused index 0 */
        448ULL,        4516ULL,        44140ULL,
        453728ULL,     4641220ULL,     47946684ULL,
        494928936ULL,  5118554604ULL,  52929477520ULL,
        547525514800ULL,5663733188684ULL,58591028515180ULL
    };

    /* Coefficients of the recurrence */
    static const int coeff[12] = {
        9, 64, -451, -1542, 7659, 16809,
        -48643, -76610, 75468, 91816,
        -33928, -30960
    };

    /* Allocate cache on first call, including first 12 terms */
    if (cache == NULL) {
        cache_cap = 13;  /* indices 0 … 12, index 0 unused */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return (seq_t)0;  /* allocation failed */
        for (size_t i = 1; i <= 12; ++i) cache[i] = init[i];
        computed = 12;
    }

    /* Expand cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return already known value */
    if (n <= computed) return cache[n];

    /* Compute new terms using the recurrence */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t sum_pos = 0, sum_neg = 0;
        for (int i = 0; i < 12; ++i) {
            idx_t idx = k - 1 - (idx_t)i;      /* a(k-i-1) */
            int c = coeff[i];
            seq_t prod = cache[idx] * (seq_t)(c > 0 ? c : -c);
            if (c > 0)
                sum_pos += prod;
            else
                sum_neg += prod;
        }
        /* For all n the recurrence guarantees sum_pos ≥ sum_neg */
        cache[k] = sum_pos - sum_neg;
    }

    computed = n;
    return cache[n];
}
