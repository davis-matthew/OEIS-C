/*
 * A199534  Number of -n..n arrays x(0..6) of 7 elements with zero sum and no
 *          two consecutive zero elements.
 * https://oeis.org/A199534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) Initial values (offset 1): a(1)=200,
 *            a(2)=6346, a(3)=53302, a(4)=252154, a(5)=860854, a(6)=2378412,
 *            a(7)=5662636 a(1) is the first term in the sequence
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 22, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* behaviour below offset is undefined */

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static bool init = false;

    /* Expand cache if necessary. */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* graceful degradation on OOM */
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise the first seven terms once. */
    if (!init) {
        cache[0] = (seq_t)200ULL;
        cache[1] = (seq_t)6346ULL;
        cache[2] = (seq_t)53302ULL;
        cache[3] = (seq_t)252154ULL;
        cache[4] = (seq_t)860854ULL;
        cache[5] = (seq_t)2378412ULL;
        cache[6] = (seq_t)5662636ULL;
        init = true;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = 8; i <= n; ++i) {
        /* Compute positive and negative contributions separately to
           avoid unsigned underflow. */
        seq_t pos = 7ULL * cache[i - 2]
                  + 35ULL * cache[i - 4]
                  + 21ULL * cache[i - 6]
                  + cache[i - 8];
        seq_t neg = 21ULL * cache[i - 3]
                  + 35ULL * cache[i - 5]
                  + 7ULL  * cache[i - 7];
        cache[i - 1] = pos - neg;   /* final value is guaranteed non‑negative */
    }

    return cache[n - 1];
}
