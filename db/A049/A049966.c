/*
 * A049966  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 3, and a(3) = 1.
 * https://oeis.org/A049966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)+a(2)+...+a(n‑1)+a(n-1-2^p), where p = ⌊log2(n-1)⌋
 *            and 2^p < n-1 ≤ 2^(p+1) (n ≥ 4). Initial values: a(1)=1, a(2)=3,
 *            a(3)=1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External linkage (required by the build). */
seq_t a(idx_t n) {
    /* Static state for memoisation. */
    static seq_t *values = NULL;     /* a array, 1‑based */
    static idx_t  cap     = 0;       /* allocated size (>= n+1) */
    static idx_t  computed = 0;      /* highest n computed so far */
    static seq_t  prefix_sum = 0;    /* sum of a[1..computed] */

    /* Ensure we have enough space for indices up to n. */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – terminate */
            abort();
        }
        values = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)1;
        } else if (i == 2) {
            val = (seq_t)3;
        } else if (i == 3) {
            val = (seq_t)1;
        } else {
            /* n ≥ 4 */
            idx_t t   = i - 1;          /* n‑1 */
            idx_t tt  = t - 1;          /* t‑1, guaranteed > 0 */
            /* Highest power of two strictly less than t. */
            idx_t pow2 = 1ull << (63 - __builtin_clzll((unsigned long long)tt));
            idx_t m = t - pow2;
            /* a(n) = prefix_sum + a(m) */
            val = prefix_sum + values[m];
        }
        values[i] = val;
        prefix_sum += val;
        computed = i;
    }

    return values[n];
}
