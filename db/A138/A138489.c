/*
 * A138489  Say what you see in previous term, from the right, reporting total
 *          number for each digit encountered. Initial term is 5.
 * https://oeis.org/A138489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(0) = 5; a(n) = next_term(a(n-1)), where next_term reports the
 *            total number of each decimal digit of its argument, read from
 *            the right, appending <count><digit> for each new digit
 *            encountered
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Compute 10^e as seq_t.  e is a small non‑negative integer. */
static seq_t pow10_idx(int e)
{
    seq_t r = 1;
    while (e-- > 0)
        r *= 10;
    return r;
}

/* Return the number of decimal digits of x. */
static int seq_digit_len(seq_t x)
{
    int len = 0;
    if (x == 0)
        return 1;
    while (x > 0) {
        x /= 10;
        len++;
    }
    return len;
}

/* Build the next term from the previous term. */
static seq_t next_term(seq_t prev)
{
    seq_t count[10] = {0};
    seq_t t = prev;

    /* Count the occurrences of each digit. */
    while (t > 0) {
        int d = (int)(t % 10);
        count[d] += 1;
        t /= 10;
    }

    bool seen[10] = {false};
    seq_t result = 0;
    t = prev;

    /* Process digits from right to left, printing each digit once. */
    while (t > 0) {
        int d = (int)(t % 10);
        if (!seen[d]) {
            seen[d] = true;
            seq_t c = count[d];
            int dlen = seq_digit_len(c);
            seq_t mult = pow10_idx(dlen + 1);               /* 10^(dlen+1) */
            result = result * mult + c * 10 + (seq_t)d;
        }
        t /= 10;
    }
    return result;
}

/* Cache for previously computed terms. */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_sz  = 0;

/* Grow the cache to at least 'needed' entries. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 128;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function implementing the sequence. */
seq_t a(idx_t n)
{
    if (!cache) {
        cache = (seq_t *)malloc(128 * sizeof(seq_t));
        if (!cache) abort();    /* allocation failure */
        cache_cap = 128;
        cache_sz  = 0;
    }

    if (cache_sz == 0) {
        cache[0] = 5;          /* a(0) = 5 */
        cache_sz = 1;
    }

    if ((size_t)n < cache_sz)
        return cache[n];

    for (size_t i = cache_sz; i <= (size_t)n; ++i) {
        ensure_cache(i + 1);
        cache[i] = next_term(cache[i - 1]);
    }
    cache_sz = (size_t)n + 1;
    return cache[(size_t)n];
}
