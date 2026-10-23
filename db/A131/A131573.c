/*
 * A131573  Numbers whose square starts with 3 identical digits.
 * https://oeis.org/A131573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   numbers whose square starts with 3 identical digits
 * Range:     u8 none, u16 n <= 1, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* helper: compute 10^e for seq_t */
static inline seq_t pow10_seq(idx_t e)
{
    seq_t p = 1;
    for (idx_t i = 0; i < e; ++i) {
        p *= 10;
    }
    return p;
}

/* helper: number of decimal digits of a seq_t (positive) */
static inline int seq_digit_len(seq_t x)
{
    int len = 0;
    while (x > 0) {
        x /= 10;
        ++len;
    }
    return len;
}

/* main sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity of cache */
    static size_t sz    = 0;   /* number of entries currently stored */
    static idx_t last_x = 1;   /* next integer to test */

    /* extend cache until we have at least n entries */
    while (sz < (size_t)n) {
        seq_t sq = (seq_t)last_x * (seq_t)last_x;

        /* determine length of sq */
        int len = seq_digit_len(sq);

        if (len >= 3) {
            /* extract the first three digits */
            idx_t exp = (idx_t)(len - 3);
            seq_t pow10 = pow10_seq(exp);
            seq_t lead = sq / pow10;

            /* check if they are identical digits (111,222,...,999) */
            if (lead >= 111 && lead % 111 == 0) {
                /* ensure enough capacity */
                if (sz == cap) {
                    size_t new_cap = cap ? cap << 1 : 4;
                    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!new_cache) return 0;               /* out‑of‑memory; caller will receive 0 */
                    cache = new_cache;
                    cap   = new_cap;
                }
                cache[sz++] = (seq_t)last_x;
            }
        }

        ++last_x;
    }

    return cache[n - 1];
}
