/*
 * A077533  Multiples of 3 using only prime digits (2, 3, 5 and 7).
 * https://oeis.org/A077533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 368, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Multiples of 3 using only prime digits (2, 3, 5, 7).
   The function enumerates all such numbers in increasing numeric order
   by generating numbers of increasing decimal length and keeping
   those whose digit sum is divisible by 3.  Results are cached so
   that any n ≥ 1 can be returned in any order of calls. */

#include <stdlib.h>

/* Static cache of generated terms. */
static seq_t *cache     = NULL;   /* array of generated numbers     */
static size_t cache_size = 0;     /* number of terms in the cache   */
static size_t cache_cap  = 0;     /* allocated capacity of the cache */

/* Next decimal length to generate. */
static idx_t next_len = 1;

/* Digit values and their mod‑3 contribution. */
static const unsigned char digit_value[4] = {2, 3, 5, 7};
static const unsigned char digit_mod3 [4] = {2, 0, 2, 1};

/* Ensure that at least n terms have been generated. */
static void ensure_cache(idx_t n)
{
    /* n is 1‑based, so we need at least n terms (cache_size >= n). */
    while (cache_size < (size_t)n)
    {
        /* Enumerate all 4^len numbers of the current length. */
        size_t total = (size_t)1 << (2 * next_len);    /* 4^len */
        for (size_t mask = 0; mask < total; ++mask)
        {
            /* Decode the 2‑bit codes for each digit (least significant first). */
            int code[20];
            size_t t = mask;
            for (idx_t i = 0; i < next_len; ++i)
            {
                code[i] = (int)(t & 3);
                t >>= 2;
            }

            /* Build the numeric value and its digit‑sum mod 3. */
            seq_t val = 0;
            int mod3 = 0;
            for (idx_t i = next_len; i > 0; --i)
            {
                int c = code[i - 1];
                val = val * (seq_t)10 + (seq_t)digit_value[c];
                mod3 += digit_mod3[c];
                mod3 %= 3;
            }

            if (mod3 == 0)
            {
                /* Grow the cache if necessary. */
                if (cache_size >= cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    if (new_cap < cache_size + 1)
                        new_cap = cache_size + 1;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp)               /* allocation failure – abort silently */
                        return;
                    cache      = tmp;
                    cache_cap  = new_cap;
                }
                cache[cache_size++] = val;
            }
        }
        /* Next iteration will generate numbers with one more digit. */
        ++next_len;
    }
}

/* Public function for OEIS sequence A077533. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n < 1, harmless fallback */
    ensure_cache(n);
    return cache[n - 1];
}
