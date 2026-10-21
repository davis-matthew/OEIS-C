/*
 * A062930  Numbers k that, when expressed in base 5 and then interpreted in
 *          base 8, give a multiple of k.
 * https://oeis.org/A062930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = k_n where k_n is the n‑th integer k ≥ 0 such that when k
 *            is written in base 5 and interpreted in base 8, the resulting
 *            number is a multiple of k. The sequence starts
 *            0,1,2,3,4,129,130,259,260,…
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* realloc */
#include <stdbool.h>  /* bool */

static seq_t *cache = NULL;         /* array of found terms */
static size_t cache_len = 0;        /* number of terms in cache      */
static size_t cache_cap = 0;        /* allocated size of the array   */
static idx_t next_candidate = 0;    /* next integer to test (0,1,2,…) */

/* grow the cache array when full */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* required external function for the sequence */
seq_t a(idx_t n)
{
    /* n is 1‑based; undefined for n == 0, but return 0 for safety */
    if (n == 0)
        return 0;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        seq_t k = (seq_t)next_candidate;   /* candidate to test         */
        bool ok = false;

        if (k == 0)
        {
            /* by definition 0 satisfies the condition */
            ok = true;
        }
        else
        {
            /* obtain base‑5 digits of k (reversed order) */
            seq_t tmp  = k;
            int digits[64];
            int dpos  = 0;

            while (tmp > 0)
            {
                digits[dpos++] = (int)(tmp % 5);
                tmp /= 5;
            }

            /* interpret these digits as a base‑8 number */
            seq_t val8 = 0;
            for (int i = dpos - 1; i >= 0; --i)
                val8 = val8 * 8 + digits[i];

            /* check the divisibility condition */
            ok = (val8 % k) == 0;
        }

        /* store k if it satisfies the condition */
        if (ok)
        {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = k;
        }

        ++next_candidate;   /* next integer to test */
    }

    return cache[n - 1];
}
