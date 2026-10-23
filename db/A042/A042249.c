/*
 * A042249  Denominators of continued fraction convergents to sqrt(650).
 * https://oeis.org/A042249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1 a(1) = 2 a(2) = 101 a(3) = 204 a(n) = 102*a(n-2) -
 *            a(n-4) for n > 3 (Recurrence from the convergents of √650)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised cache of the sequence */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;

/* Ensure that seq_cache holds at least indices 0..n (inclusive) */
static void ensure_size(idx_t n)
{
    if (n + 1 <= cache_len) return;          /* already computed */

    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);           /* allocation failure */
    seq_cache = tmp;

    /* compute new entries sequentially */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i == 0) {
            seq_cache[i] = 1;
        } else if (i == 1) {
            seq_cache[i] = 2;
        } else if (i == 2) {
            seq_cache[i] = 101;
        } else if (i == 3) {
            seq_cache[i] = 204;
        } else {
            /* recurrence: a(n) = 102*a(n-2) - a(n-4) */
            seq_cache[i] = (seq_t)102 * seq_cache[i - 2] - seq_cache[i - 4];
        }
    }
    cache_len = new_len;
}

/* External function required by the platform */
seq_t a(idx_t n)
{
    ensure_size(n);
    return seq_cache[n];
}
