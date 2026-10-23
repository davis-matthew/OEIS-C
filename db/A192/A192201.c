/*
 * A192201  Monotonic ordering of nonnegative differences 5^i-10^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth nonnegative difference 5^i - 10^j, with 0 ≤ i ≤ 40
 *            and j ≥ 0. The terms are ordered increasingly (duplicates
 *            removed)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Local comparison routine for qsort. */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Return the n‑th term (a(1)=0).  Calls in any order are handled
   correctly; the sequence is generated only once. */
seq_t a(idx_t n)
{
    /* Static cache – generated on first call. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (!cache) {
        /* Initial capacity – plenty for i ≤ 40, j ≤ 27. */
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) /* allocation failure – abort */ exit(1);
        size_t len = 0;

        /* Generate all differences. */
        seq_t pow5 = 1;                          /* 5^i */
        for (idx_t i = 0; i <= 40; ++i) {
            seq_t pow10 = 1;                     /* 10^j */
            for (;;) {
                if (pow10 > pow5) break;          /* 10^j > 5^i → stop this j-loop */

                /* Store the difference 5^i - 10^j. */
                if (len == cache_cap) {
                    /* Grow the cache. */
                    cache_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) exit(1);
                    cache = tmp;
                }
                cache[len++] = pow5 - pow10;

                /* Advance to next 10^j. */
                seq_t next_pow10 = pow10 * 10;
                if (next_pow10 < pow10) break;     /* overflow in seq_t → cannot continue */
                pow10 = next_pow10;
            }

            /* Advance to next 5^i. */
            seq_t next_pow5 = pow5 * 5;
            if (next_pow5 < pow5) break;           /* overflow → further i produce no new values */
            pow5 = next_pow5;
        }

        /* Sort the values. */
        qsort(cache, len, sizeof(seq_t), cmp_seq);

        /* Remove duplicates in‑place. */
        size_t uniq = 0;
        for (size_t i = 0; i < len; ++i) {
            if (uniq == 0 || cache[i] != cache[uniq - 1]) {
                cache[uniq++] = cache[i];
            }
        }
        cache_len = uniq;
    }

    if (n == 0) return (seq_t)0;                                     /* offset not defined */
    size_t idx = (size_t)(n - 1);
    if (idx < cache_len) return cache[idx];
    return (seq_t)0;                                                 /* out of range */
}
