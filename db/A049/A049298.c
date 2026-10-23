/*
 * A049298  Take reduced residue systems of n, generate its first differences,
 *          dRRS(n); sequence gives maximal value of dRSSS(n).
 * https://oeis.org/A049298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (maximal consecutive run of numbers in 0..n-1 that
 *            are not coprime to n). For n <= 2 (φ(n) = 1) there is no
 *            difference, so a(n) = 0
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
    /* φ(n) ≤ 1 only for n = 1, 2 */
    if (n <= 2) return (seq_t)0;

    /* Flag array: true if i is NOT coprime to n (including i = 0) */
    char *bad = (char*)malloc((size_t)n);
    if (!bad) return (seq_t)0;   /* allocation failure – safe fallback */

    for (idx_t i = 0; i < n; ++i)
        bad[i] = (gcd_idx(i, n) != 1);

    idx_t max_run = 0, cur = 0;

    /* Duplicate the circle to handle wrapping runs. */
    for (idx_t i = 0; i < 2 * n; ++i) {
        idx_t idx = i % n;
        if (bad[idx]) {
            ++cur;
        } else {
            if (cur > max_run) max_run = cur;
            cur = 0;
        }
    }
    if (cur > max_run) max_run = cur;   /* in case the max run ends at the last element */

    free(bad);
    return (seq_t)(max_run + 1);
}
