/*
 * A061910  Positive numbers k such that sum of digits of k^2 is a square.
 * https://oeis.org/A061910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 134, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A061910 – Positive integers k such that the sum of the digits of k^2
   is a perfect square.
   The sequence is generated on demand by searching for successive integers k.
   Intermediate values are kept in a small cache so that repeated or out‑of‑order
   calls to a(idx_t n) are efficient.
   All arithmetic is performed on the build‑time type seq_t; a simple integer
   square‑root routine is implemented inline to avoid missing helper headers.
*/

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache      = NULL;   /* cached terms of the sequence */
static size_t cache_len  = 0;      /* number of cached terms */
static size_t cache_cap  = 0;      /* capacity of the cache array */
static seq_t  next_k     = 1;      /* next value of k to test */

static void cache_append(seq_t val)
{
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* integer square‑root: floor(sqrt(x))  (works for any seq_t type) */
static seq_t isqrt(seq_t x)
{
    seq_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        seq_t mid = (lo + hi) / 2;
        seq_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            ans = mid;
            lo  = mid + 1;
        } else {
            if (mid == 0) break;
            hi  = mid - 1;
        }
    }
    return ans;
}

/* test whether s is a perfect square */
static bool is_square(seq_t s)
{
    seq_t r = isqrt(s);
    return r * r == s;
}

/* sum of decimal digits of a seq_t value */
static seq_t digit_sum(seq_t v)
{
    seq_t sum = 0;
    while (v != 0) {
        sum += v % 10;
        v   /= 10;
    }
    return sum;
}

/* return the nth term of the sequence, 1‑based offset */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 as a safeguard */

    /* serve from cache if available */
    if ((idx_t)cache_len >= n) return cache[n - 1];

    /* generate terms until we have the nth */
    while ((idx_t)cache_len < n) {
        seq_t sq   = next_k * next_k;
        seq_t dsum = digit_sum(sq);
        if (is_square(dsum)) cache_append(next_k);
        next_k++;
    }
    return cache[n - 1];
}
