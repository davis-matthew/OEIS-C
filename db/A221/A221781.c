/*
 * A221781  Number of 2Xn arrays of occupancy after each element stays put or
 *          moves to some horizontal, diagonal or antidiagonal neighbor,
 *          without move-in move-out left turns.
 * https://oeis.org/A221781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A221781: Number of 2×n arrays of occupancy after each element stays put or moves to some horizontal, diagonal or antidiagonal neighbor, without move‑in move‑out left turns.
 * Recurrence (valid for n ≥ 17):
 *   a(n) = 19*a(n-1) -119*a(n-2) +340*a(n-3) -447*a(n-4) +231*a(n-5) -367*a(n-6) +1636*a(n-7)
 *          -2994*a(n-8) +3520*a(n-9) -4119*a(n-10) +2955*a(n-11) +294*a(n-12) -1044*a(n-13)
 *          +55*a(n-14) +59*a(n-15) +2*a(n-16)
 * Initial values:
 *   a(1) = 1
 *   a(2) = 25
 *   a(3) = 258
 *   a(4) = 2631
 *   a(5) = 26861
 *   a(6) = 273816
 *   a(7) = 2792384
 *   a(8) = 28476902
 *   a(9) = 290412831
 *   a(10) = 2961693438
 *   a(11) = 30204010646
 *   a(12) = 308027296692
 *   a(13) = 3141331776218
 *   a(14) = 32036009453063
 *   a(15) = 326710444860258
 *   a(16) = 3331866751461036
 */

#include <stdlib.h>

static const seq_t init_vals[16] = {
    (seq_t)1,
    (seq_t)25,
    (seq_t)258,
    (seq_t)2631,
    (seq_t)26861,
    (seq_t)273816,
    (seq_t)2792384,
    (seq_t)28476902,
    (seq_t)290412831,
    (seq_t)2961693438,
    (seq_t)30204010646,
    (seq_t)308027296692,
    (seq_t)3141331776218,
    (seq_t)32036009453063,
    (seq_t)326710444860258,
    (seq_t)3331866751461036
};

static const int coeffs[16] = {
    19, -119, 340, -447, 231, -367, 1636, -2994,
    3520, -4119, 2955, 294, -1044, 55, 59, 2
};

static seq_t *cache = NULL;
static idx_t cache_max = 0;  /* largest n for which cache[n] is valid */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for offset 1, but safe fallback */

    if (cache == NULL) {
        /* Allocate space for at least one element (index 0 unused) */
        cache = (seq_t*)malloc(sizeof(seq_t) * (n + 1));
        if (!cache) return (seq_t)0; /* allocation failure -> zero */
        cache_max = 0;
    } else if (cache_max < n) {
        /* Reallocate to accommodate larger n */
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cache_max = 0; /* will recompute missing values below */
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 16) {
            /* Seed values */
            cache[i] = init_vals[i - 1];
        } else {
            /* Use the recurrence */
            seq_t positive = 0, negative = 0;
            for (int j = 0; j < 16; ++j) {
                idx_t idx   = i - (j + 1);
                seq_t term  = (seq_t)(j >= 0 ? (unsigned)abs(coeffs[j]) : 0) * cache[idx];
                if (coeffs[j] > 0) {
                    positive += term;
                } else {
                    negative += term;
                }
            }
            cache[i] = positive - negative;
        }
    }
    cache_max = n;
    return cache[n];
}
