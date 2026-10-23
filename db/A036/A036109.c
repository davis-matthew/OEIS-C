/*
 * A036109  A summarize Fibonacci sequence starting with a(0)=2 and a(1)=1:
 *          summarize the previous two terms!.
 * https://oeis.org/A036109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2, a(1)=1.
   For n≥2, a(n) is obtained by “summarizing” the digits of
   a(n−1) and a(n−2).  Count how many times each decimal digit
   d (0–9) occurs in the two previous terms; for each d from 9
   down to 0 with a non‑zero count, append the decimal
   representation of that count followed by the digit d. */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* static cache of computed terms   */
static idx_t cache_size = 0;    /* number of terms currently stored */
static size_t cache_cap  = 0;    /* allocated capacity                 */

/* Compute 10^k as a seq_t value.  k is small (≤ number of digits of a count). */
static seq_t pow10_int(int k)
{
    seq_t r = 1;
    for (int i = 0; i < k; ++i)
        r *= 10;
    return r;
}

/* Count the digits of x into the array counts[10].  The array
   must be zeroed before the call. */
static void count_digits(seq_t x, unsigned int counts[10])
{
    while (x != 0)
    {
        unsigned int d = (unsigned int)(x % 10);
        counts[d]++;          /* record one occurrence of digit d */
        x /= 10;
    }
}

/* Grow the cache so that it can hold at least indices 0 … n. */
static void ensure_cache_cap(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n + 1)
        new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise base cases on first call. */
    if (cache_size == 0)
    {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        cache_size = 2;
        cache_cap  = 2;
    }

    /* Extend the cache up to n. */
    if (n >= cache_size)
    {
        ensure_cache_cap(n);
        for (idx_t i = cache_size; i <= n; ++i)
        {
            unsigned int counts[10] = {0};
            count_digits(cache[i-1], counts);
            count_digits(cache[i-2], counts);

            seq_t res = 0;
            for (int d = 9; d >= 0; --d)
            {
                unsigned int cnt = counts[d];
                if (cnt == 0)
                    continue;

                /* Number of decimal digits in cnt. */
                unsigned int len = 0;
                unsigned int tmp = cnt;
                do { ++len; tmp /= 10; } while (tmp);

                seq_t pow = pow10_int(len + 1);   /* 10^(len+1) */
                res = res * pow + (seq_t)cnt * 10 + d;
            }
            cache[i] = res;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
