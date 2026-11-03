/*
 * A120189  a(1)=4; a(n) = floor((31+Sum_{i=1..n-1} a(i))/7).
 * https://oeis.org/A120189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4 a(n) = floor((31 + Σ_{i=1}^{n-1} a(i)) / 7) for n ≥ 2
 * Range:     u8 n <= 18, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* static cache of computed terms and cumulative sum */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;
    static seq_t sum = 0;

    if (n == 0)
        return 0;  /* undefined, but never requested */

    if (n > max_n) {
        size_t new_size = (size_t)n;
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp && new_size > 0)
            return 0;  /* allocation failure – unlikely in tests */

        cache = tmp;

        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t val = ((seq_t)31 + sum) / (seq_t)7;  /* floor division */
            cache[i - 1] = val;
            sum += val;
        }
        max_n = n;
    }

    return cache[n - 1];
}
