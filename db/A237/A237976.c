/*
 * A237976  Number of strict partitions of n such that (least part) < number
 *          of parts.
 * https://oeis.org/A237976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of strict partitions of n with least part < number
 *            of parts
 * Range:     u8 n <= 30, u16 n <= 79, u32 n <= 237, u64 n <= 791,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* compute maximal k such that k*(k-1)/2 <= n */
    idx_t max_k = 0;
    for (idx_t k = 1; ; ++k) {
        if (k * (k - 1) / 2 > n) break;
        max_k = k;
    }
    if (max_k == 0) return 0;

    /* allocate arrays */
    size_t sz = (size_t)(n + 1);
    seq_t *dp_atmost = (seq_t*)calloc(sz, sizeof(seq_t));     /* partitions with parts <= current k */
    seq_t *prev_exact = (seq_t*)calloc(sz, sizeof(seq_t));    /* p_exact(n, k-1) */
    seq_t *curr_exact = (seq_t*)calloc(sz, sizeof(seq_t));    /* p_exact(n, k) */

    if (!dp_atmost || !prev_exact || !curr_exact) { /* allocation failure */
        /* In a real system you would handle this error. */
        return 0;
    }

    dp_atmost[0] = 1;       /* one way to partition 0 */
    prev_exact[0] = 1;      /* p_exact(0,0) = 1 */

    seq_t result = 0;

    for (idx_t k = 1; k <= max_k; ++k) {
        /* update atmost array: partitions into parts <= k */
        for (idx_t s = k; s <= n; ++s) {
            dp_atmost[s] = dp_atmost[s] + dp_atmost[s - k];
        }

        /* compute p_exact(n, k) */
        curr_exact[0] = 0;
        for (idx_t t = 1; t <= n; ++t) {
            curr_exact[t] = prev_exact[t - 1];
            if (t >= k) {
                curr_exact[t] = curr_exact[t] + curr_exact[t - k];
            }
        }

        /* contribution for this k */
        idx_t m = n - k * (k - 1) / 2;
        seq_t add = curr_exact[m];
        if (m >= k * k) {
            add = add - dp_atmost[m - k * k];
        }
        result = result + add;

        /* swap exact arrays for next iteration */
        seq_t *tmp = prev_exact;
        prev_exact = curr_exact;
        curr_exact = tmp;
    }

    free(dp_atmost);
    free(prev_exact);
    free(curr_exact);

    return result;
}
