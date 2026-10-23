/*
 * A130216  a(0) = 3; a(n) = a(n-1) + (number of multiples of 3 so far in the
 *          sequence).
 * https://oeis.org/A130216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 3; a(n) = a(n-1) + (number of multiples of 3 among
 *            a[0..n-1])
 * Range:     u8 n <= 33, u16 n <= 552, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for computed terms and counts of multiples of 3. */
static seq_t *value = NULL;      /* a(n) values */
static idx_t *count3 = NULL;     /* c(n) = # multiples of 3 up to n */
static idx_t cache_len = 0;      /* number of elements stored (>=1 if any data) */

/* Compute the n-th term of sequence A130216.   */
seq_t a(idx_t n)
{
    /* Ensure the cache covers index n. */
    idx_t needed = n + 1;
    if (cache_len < needed) {
        /* Grow the storage vectors. */
        void *tmp;

        tmp = realloc(value, needed * sizeof(seq_t));
        if (!tmp) return 0;                 /* out‑of‑memory: return 0 as a safe fallback */
        value = (seq_t *)tmp;

        tmp = realloc(count3, needed * sizeof(idx_t));
        if (!tmp) return 0;
        count3 = (idx_t *)tmp;

        /* Initialize the very first term if it does not exist yet. */
        if (cache_len == 0) {
            value[0] = (seq_t)3;
            count3[0] = 1;                  /* 3 is a multiple of 3 */
            cache_len = 1;
        }

        /* Fill missing entries up to 'needed - 1'. */
        for (idx_t i = cache_len; i < needed; ++i) {
            seq_t prev_val   = value[i - 1];
            idx_t  prev_cnt  = count3[i - 1];

            /* a[i] = a[i-1] + (#multiples of 3 up to i-1) */
            value[i] = prev_val + (seq_t)prev_cnt;

            /* Does the new value itself add to the multiple count? */
            idx_t add = ((value[i] % (seq_t)3) == (seq_t)0) ? 1 : 0;
            count3[i] = prev_cnt + add;
        }
        cache_len = needed;
    }

    return value[n];
}
