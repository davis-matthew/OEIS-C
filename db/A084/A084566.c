/*
 * A084566  a(0)=1, a(2n) = 2a(2n-1)+a(n), a(2n+1) = 2a(2n)+2a(n).
 * https://oeis.org/A084566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(2n) = 2*a(2n-1) + a(n); a(2n+1) = 2*a(2n) + 2*a(n);
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following code implements the recurrence with memoisation. */
/* It is valid C11 and C++17. */

#include <stddef.h>
#include <stdlib.h>

/* remember that seq_t and idx_t are supplied by the build */
static seq_t *cache = NULL;      /* memoisation table */
static size_t cache_len = 0;     /* current allocated length */

/* Resize cache to hold indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if (cache_len <= (size_t)n) {
        size_t new_len = (cache_len > 0) ? cache_len * 2 : 8;
        while (new_len <= (size_t)n) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                      /* memory allocation failure */
        /* Initialize new slots to zero (sentinel value). */
        for (size_t i = cache_len; i < new_len; ++i) tmp[i] = 0;
        cache = tmp;
        cache_len = new_len;
    }
}

/* Recursive core with memoisation. */
static seq_t a_rec(idx_t n)
{
    ensure_cache_size(n);

    if (n == (idx_t)0) {
        cache[0] = (seq_t)1;
        return cache[0];
    }

    seq_t cached = cache[n];
    if (cached != (seq_t)0) return cached;

    seq_t a_prev = a_rec(n - 1);     /* value of a(n-1) */

    seq_t res;
    if ((n & (idx_t)1) == (idx_t)0) {          /* n even: n = 2k */
        seq_t a_half = a_rec(n >> 1);          /* a(k) */
        res = a_prev + a_prev + a_half;        /* 2*a_prev + a_half */
    } else {                                  /* n odd: n = 2k+1 */
        seq_t a_half = a_rec((n - 1) >> 1);    /* a(k) */
        res = a_prev + a_prev + a_half + a_half; /* 2*a_prev + 2*a_half */
    }

    cache[n] = res;
    return res;
}

/* Public entry point required by the judge. */
seq_t a(idx_t n)
{
    return a_rec(n);
}
