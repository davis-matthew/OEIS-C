/*
 * A117155  List of smaller members p of the prime pairs (p,q) mentioned in
 *          A117300.
 * https://oeis.org/A117155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smaller member of a consecutive prime pair (p, q)
 *            where p < q, p and q have the same number of decimal digits d,
 *            and q - p = d. The sequence is computed by enumerating primes in
 *            increasing order and storing each p that satisfies the condition
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   digit_count, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_count.h"
#include "../utils/is_prime.h"

/* ---------- cache for computed terms ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* add a value to the cache, expanding the array as required */
static void add_term(seq_t val)
{
    if (cache_len == cache_cap)
    {
        cache_cap = cache_cap == 0 ? 256 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
    }
    cache[cache_len++] = val;
}

/* ---------- state kept between calls ---------- */
static uint64_t last_checked = 1;   /* last number examined */
static uint64_t prev_prime   = 0;   /* last prime found */

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    /* generate terms until we have at least n of them */
    while (cache_len < n)
    {
        /* find next prime number */
        /* start from the smallest candidate that can be prime */
        if (last_checked < 2)
            last_checked = 2;
        else if (last_checked == 2)
            last_checked = 3;
        else
            last_checked += 2;        /* skip even numbers */

        while (!is_prime((idx_t)last_checked))
        {
            last_checked += 2;
        }

        /* last_checked now holds a prime */
        if (prev_prime != 0)
        {
            uint64_t d_prev = digit_count(prev_prime);
            if (digit_count(last_checked) == d_prev &&
                (last_checked - prev_prime == d_prev))
            {
                add_term((seq_t)prev_prime);
                if (cache_len >= n) break;
            }
        }

        /* advance state for the next iteration */
        prev_prime = last_checked;
    }

    return cache[n - 1];
}
