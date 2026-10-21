/*
 * A095984  Numbers k such that k * (largest digit of k) is a palindrome.
 * https://oeis.org/A095984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 175, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerates integers k such that k * largest_digit(k) is a
   palindrome in decimal.  a(1) = 0, a(2) = 1, a(3) = 2, …   */
/* This implementation caches found terms and scans candidates in
   increasing order.  It works for any seq_t that supports usual
   arithmetic and for any idx_t that is an unsigned integer. */

#include <stdlib.h>
#include <stdbool.h>

/* Cache of discovered terms. */
static seq_t *cache        = NULL;   /* array of stored k values       */
static idx_t  cache_size   = 0;      /* number of terms cached         */
static idx_t  cache_cap    = 0;      /* allocated number of slots      */
static idx_t  next_candidate = 0;    /* next integer to test (≥0)      */

/* Ensure the cache has room for at least `need` entries. */
static void ensure_cache_cap(idx_t need)
{
    if (need <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                           /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the largest decimal digit of x. */
static inline seq_t largest_digit(seq_t x)
{
    seq_t max = 0;
    if (x == 0)
        return 0;
    while (x > 0) {
        seq_t d = x % 10;
        if (d > max)
            max = d;
        x /= 10;
    }
    return max;
}

/* Return true if v is a palindrome in base‑10. */
static inline bool is_palindrome(seq_t v)
{
    seq_t orig = v, rev = 0;
    while (orig > 0) {
        rev = rev * 10 + (orig % 10);
        orig /= 10;
    }
    return rev == v;
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined for offset 0 */

    while (cache_size < n) {
        seq_t k = (seq_t)next_candidate;
        seq_t ld = largest_digit(k);
        seq_t prod = k * ld;
        if (is_palindrome(prod)) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = k;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
