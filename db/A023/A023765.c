/*
 * A023765  Nialpdromes: digits in base 9 are in nonincreasing order.
 * https://oeis.org/A023765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A023765: numbers whose base‑9 digits are in non‑increasing order. */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static const int MAX_LEN = 8;

/* compare function for qsort */
static int seq_cmp(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* recursive generation of numbers with given length */
static void rec(seq_t val, int last_digit, int pos, int len)
{
    if (pos == len)
    {
        cache[cache_size++] = val;
        return;
    }
    for (int d = 0; d <= last_digit; ++d)
    {
        rec(val * ((seq_t)9) + (seq_t)d, d, pos + 1, len);
    }
}

/* build the cache of all terms up to length MAX_LEN */
static void build_cache(void)
{
    /* approximate total count for length up to MAX_LEN (≈24301) */
    cache = (seq_t *)malloc(25000 * sizeof(seq_t));
    cache_size = 0;

    /* number 0 */
    cache[cache_size++] = (seq_t)0;

    for (int len = 1; len <= MAX_LEN; ++len)
    {
        /* first digit from 1..8 (leading zero not allowed except for 0 itself) */
        for (int first = 1; first <= 8; ++first)
        {
            rec((seq_t)first, first, 1, len);
        }
    }

    /* sort all values to obtain the sequence in increasing order */
    qsort(cache, cache_size, sizeof(seq_t), seq_cmp);
}

/* main sequence function */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized)
    {
        build_cache();
        initialized = 1;
    }

    /* we assume n >= 1 and n <= cache_size */
    return cache[(size_t)(n - 1)];
}
