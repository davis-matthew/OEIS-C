/*
 * A081137  Duplicate of A027474.
 * https://oeis.org/A081137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = C(n,2) * 7^(n-2) (C(n,2)= n*(n-1)/2)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *pow7_cache = NULL;
static idx_t pow7_cache_len = 0;

static void
ensure_pow7(idx_t e)
{
    if (pow7_cache_len <= e) {
        if (pow7_cache_len == 0) {
            pow7_cache = (seq_t *)malloc(sizeof(seq_t));
            if (!pow7_cache) return;
            pow7_cache[0] = (seq_t)1;
            pow7_cache_len = 1;
        }
        while (pow7_cache_len <= e) {
            seq_t next = pow7_cache[pow7_cache_len - 1] * (seq_t)7;
            seq_t *tmp = (seq_t *)realloc(pow7_cache,
                                          (pow7_cache_len + 1) * sizeof(seq_t));
            if (!tmp) return;
            pow7_cache = tmp;
            pow7_cache[pow7_cache_len++] = next;
        }
    }
}

seq_t
a(idx_t n)
{
    if (n < 2) return 0;

    idx_t exp = n - 2;
    ensure_pow7(exp);

    seq_t binom = (seq_t)n * ((seq_t)(n - 1)) / 2;
    return binom * pow7_cache[exp];
}
