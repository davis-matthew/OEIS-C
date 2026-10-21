/*
 * A061658  Numbers k > 1 such that, in base 4, k and k^2 contain the same
 *          digits in the same proportion.
 * https://oeis.org/A061658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 325, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS formula is: a(n) is the n‑th integer k > 1 such that k and
   k² have the same multiset of base‑4 digits up to a common multiple.
   In other words the digit proportions of k and k² are equal. */

#include <stdlib.h>

static void digit_counts(seq_t x, unsigned cnt[4], unsigned *len)
{
    *len = 0;
    while (x != 0)
    {
        unsigned d = (unsigned)(x % 4);   /* last base‑4 digit */
        ++cnt[d];
        x /= 4;
        ++(*len);
    }
}

static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap * 2);
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for n=0 */

    /* static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static seq_t next_k      = 2;              /* candidate to test (k>1) */

    /* generate terms until we have at least n of them */
    while (cache_len < n)
    {
        seq_t k  = next_k++;                   /* candidate */
        seq_t k2 = k * k;                      /* square */

        unsigned cnt1[4] = {0}, cnt2[4] = {0};
        unsigned len1 = 0, len2 = 0;

        digit_counts(k , cnt1, &len1);
        digit_counts(k2, cnt2, &len2);

        /* check if the digit proportions are identical */
        int ok = 1;
        for (unsigned d = 0; d < 4 && ok; ++d)
        {
            seq_t left  = (seq_t)cnt1[d] * (seq_t)len2;
            seq_t right = (seq_t)cnt2[d] * (seq_t)len1;
            if (left != right) ok = 0;
        }
        if (ok)
        {
            ensure_capacity(cache_len + 1, &cache_cap, &cache);
            cache[cache_len++] = k;
        }
    }

    return cache[n - 1];
}
