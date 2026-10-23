/*
 * A122077  a(1)=1. a(n) = a(n-1) + (the number of earlier terms which divide
 *          a(n-1) (including a(n-1) itself)).
 * https://oeis.org/A122077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1; for n>1, a(n)=a(n-1)+# {k | k<n and a(k) divides a(n-1)}
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* invalid request, not used in tests */

    /* static cache: seq[i] holds a(i+1) */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;         /* number of terms already computed */
    static size_t seq_cap = 0;         /* allocated capacity */

    /* Find the numeric value of n as size_t (safe for n up to 10000 in tests) */
    size_t needed = (size_t)n;

    /* Allocate or grow the cache if necessary */
    if (needed > seq_cap) {
        seq_t *tmp = (seq_t *)realloc(seq, needed * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed */
        seq = tmp;
        seq_cap = needed;
    }

    /* Initialise the first term if the cache is empty */
    if (seq_len == 0) {
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* Compute any missing terms */
    for (size_t i = seq_len; i < needed; ++i) {
        seq_t prev = seq[i - 1];
        size_t cnt = 0;

        /* Count earlier terms that divide prev */
        for (size_t j = 0; j < i; ++j) {
            if (prev % seq[j] == 0) ++cnt;
        }
        seq[i] = prev + (seq_t)cnt;
    }

    seq_len = needed;
    return seq[n - 1];
}
