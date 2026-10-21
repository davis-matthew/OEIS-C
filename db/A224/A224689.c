/*
 * A224689  Number of (n+4) X 11 0..1 matrices with each 5 X 5 subblock
 *          idempotent.
 * https://oeis.org/A224689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 57, u64 n <= 136, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A224689: Number of (n+4)×11 0..1 matrices with each 5×5 subblock idempotent.  */
/*  Recurrence (valid for n > 12):                                          */
/*    a(n) = 4*a(n‑1) – 6*a(n‑2) + 4*a(n‑3) – a(n‑4) + a(n‑5) – 3*a(n‑6)  */
/*           + 3*a(n‑7) – a(n‑8).                                           */
/*  Initial values (n = 1 … 12):                                          */
/*    6713, 2591, 2862, 3214, 3540, 3819, 4572, 5723, 7181, 8868, 10729, 13235 */

#include <stdlib.h>

static const seq_t init_vals[12] = {
    (seq_t)6713,  (seq_t)2591,  (seq_t)2862,  (seq_t)3214,
    (seq_t)3540,  (seq_t)3819,  (seq_t)4572,  (seq_t)5723,
    (seq_t)7181,  (seq_t)8868,  (seq_t)10729, (seq_t)13235
};

seq_t a(idx_t n) {
    if (n == 0) return 0;                           /* undefined, safe fallback */

    /* Lazy cache: cache[i] holds a(i+1).  */
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* number of elements allocated */
    static size_t max   = 0;    /* largest n that has been computed  */

    /* Ensure base values are stored.  */
    if (max < 12) {
        size_t need = 12;
        if (need > cap) {
            seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cap   = need;
        }
        for (idx_t i = 0; i < 12; ++i) cache[i] = init_vals[i];
        max = 12;
    }

    /* Return directly if we already know the value.  */
    if (n <= max) return cache[n-1];

    /* Grow the cache to accommodate up to n.  */
    if (n > cap) {
        size_t new_cap = cap ? cap : 16;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute values from (max+1) up to n using the recurrence. */
    for (idx_t k = max + 1; k <= n; ++k) {
        /* a(k) depends on a(k‑1) … a(k‑8).  */
        seq_t pos = 0, neg = 0;
        pos += (seq_t)4 * cache[k-2];
        pos += (seq_t)4 * cache[k-4];
        pos += cache[k-6];
        pos += (seq_t)3 * cache[k-8];
        neg += (seq_t)6 * cache[k-3];
        neg += cache[k-5];
        neg += (seq_t)3 * cache[k-7];
        neg += cache[k-9];
        cache[k-1] = pos - neg;    /* pos >= neg for all valid k */
    }
    max = n;
    return cache[n-1];
}
