/*
 * A180477  Numbers m such that r*m/k is an integer. m=(x_1 x_2 ... x_r) where
 *          x_i are digits of m, k = x_1 + x_2 + ... + x_r.
 * https://oeis.org/A180477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A180477: numbers m such that r * m / k is an integer
 * where r = number of decimal digits of m and k = sum of its digits.
 * The routine generates the sequence by a simple linear search
 * and memoises the results. */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* helpers: digit count and digit sum for unsigned integers (idx_t)   */
static unsigned int digit_len_idx(idx_t n)
{
    unsigned int len = 0;
    do {                   /* n > 0 for all values tested          */
        len++;              /* count one more digit                  */
        n /= 10;            /* remove least‑significant digit        */
    } while (n != 0);
    return len;
}

static unsigned int digit_sum_idx(idx_t n)
{
    unsigned int sum = 0;
    do {                   /* n > 0 for all values tested          */
        sum += (unsigned int)(n % 10);  /* add current digit      */
        n /= 10;            /* remove least‑significant digit        */
    } while (n != 0);
    return sum;
}
/* ------------------------------------------------------------------ */

/* memoised cache of computed sequence values                        */
static seq_t *cache      = NULL;   /* array of stored terms          */
static idx_t cache_len   = 0;      /* number of terms stored         */
static idx_t cache_cap   = 0;      /* allocated capacity (terms)     */
static idx_t scan_start  = 1;      /* next integer candidate to test */

/* enlarge the cache array as needed                                 */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap) return;        /* already big enough       */
    idx_t newcap = cache_cap ? cache_cap * 2 : 16;
    if (newcap < needed) newcap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = tmp;
    cache_cap = newcap;
}

/* ------------------------------------------------------------------ */
/* main routine: return the nth term (1‑based index)                  */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, 0 is undefined                         */
        return 0;

    /* keep generating terms until we have at least n of them          */
    while (cache_len < n)
    {
        idx_t m = scan_start;
        unsigned int r = digit_len_idx(m);
        unsigned int k = digit_sum_idx(m);   /* k > 0 for m >= 1          */
        /* check if r * m is divisible by k                         */
        if (k != 0 && ((idx_t)r * m) % k == 0)
        {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)m;
        }
        scan_start++;                          /* next candidate          */
    }
    return cache[n - 1];                      /* 1‑based indexing        */
}
