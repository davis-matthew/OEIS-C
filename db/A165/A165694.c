/*
 * A165694  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=43, a(2)=1806, a(3)=75852, a(4)=3185784,
 *            a(5)=133802928, a(6)=5619722976, a(7)=236028364992,
 *            a(8)=9913191329664; For n≥9: a(n) = 41·(a(n−1)+a(n−2)+…+a(n−8))
 *            − 861·a(n−9) and for n=9 the constant term +1 must be added (the
 *            generating‑function denominator gives a non‑homogeneous
 *            equati...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Permanent cache of previously computed values.  */
    static seq_t *cache      = NULL;   /* pointer to the array */
    static idx_t   capacity  = 0;      /* allocated size of the array */
    static idx_t   max_comp  = 0;      /* largest index for which cache[i] is valid */

    /* Initial values for 0 ≤ n ≤ 8. */
    static const seq_t init_vals[9] = {
        (seq_t)1ULL,              /* a(0) */
        (seq_t)43ULL,             /* a(1) */
        (seq_t)1806ULL,           /* a(2) */
        (seq_t)75852ULL,          /* a(3) */
        (seq_t)3185784ULL,        /* a(4) */
        (seq_t)133802928ULL,      /* a(5) */
        (seq_t)5619722976ULL,     /* a(6) */
        (seq_t)236028364992ULL,   /* a(7) */
        (seq_t)9913191329664ULL   /* a(8) */
    };

    /* First allocation (if necessary).  */
    if (capacity == 0) {
        capacity = 64;                                   /* a reasonable start size   */
        cache    = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;                     /* allocation failure        */
        memset(cache, 0, capacity * sizeof(seq_t));
        memcpy(cache, init_vals, sizeof(init_vals));
        max_comp = 8;                                    /* last computed index      */
    }

    /* Return early if the desired value is already cached.  */
    if (n <= max_comp) return cache[n];

    /* Grow the cache if it does not yet hold index n. */
    if (n >= capacity) {
        idx_t new_cap = capacity;
        while (new_cap <= n) new_cap <<= 1;              /* double until large enough */
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;                /* allocation failure */
        memcpy(new_cache, cache, capacity * sizeof(seq_t));
        memset(new_cache + capacity, 0,
               (new_cap - capacity) * sizeof(seq_t));
        free(cache);
        cache    = new_cache;
        capacity = new_cap;
    }

    /* Compute values on demand, starting just after the last computed one. */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        /* Sum the 8 preceding terms. */
        seq_t sum = 0;
        for (idx_t k = 1; k <= 8; ++k) {
            sum += cache[i - k];
        }

        /* Main recurrence part. */
        seq_t val = (seq_t)41 * sum - (seq_t)861 * cache[i - 9];

        /* Add the +1 constant only for n == 9. */
        if (i == 9) {
            val += (seq_t)1;
        }

        cache[i] = val;
    }

    /* Update the index of the last computed element. */
    max_comp = n;

    return cache[n];
}
