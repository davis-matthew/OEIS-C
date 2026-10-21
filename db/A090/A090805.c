/*
 * A090805  A simple recurrence with one error.
 * https://oeis.org/A090805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, dumb
 * Formula:   a(0) = 1; a(n) = n * (a(n-1) + 1) for n != 4 a(4) = a(3) * 4 + 1
 *            (i.e. the recurrence has a special error at n = 4)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static idx_t calc_len = 0;          /* number of pre‑computed entries          */
    static seq_t *cache = NULL;         /* cache of already computed a(i) values   */

    /* Allocate/reallocate cache big enough for indices 0..n */
    if (n >= calc_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();              /* out of memory, let program terminate   */
        cache = tmp;
        /* Compute missing values */
        for (idx_t i = calc_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 4) {
                cache[i] = cache[i - 1] * (seq_t)4 + (seq_t)1;
            } else {
                cache[i] = (seq_t)i * (cache[i - 1] + (seq_t)1);
            }
        }
        calc_len = n + 1;
    }
    return cache[n];
}
