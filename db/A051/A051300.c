/*
 * A051300  a(n) = a(n-1) + rotate( a(n-1), 1 digit right), a(1) = 1.
 * https://oeis.org/A051300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1, a(n) = a(n-1) + rotate(a(n-1), 1 digit right)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Count decimal digits of a number (at least one digit) */
static int digit_len_seq_t(seq_t x)
{
    int len = 0;
    do {
        len++;
        x /= 10;
    } while (x != 0);
    return len;
}

/* Rotate a number one digit to the right:
       abcde -> eabcd   (last digit becomes first) */
static seq_t rotate_digit_right(seq_t x)
{
    seq_t last = x % 10;     /* last digit */
    seq_t rest = x / 10;     /* the remaining digits */

    int digits = digit_len_seq_t(x);
    seq_t pow10 = 1;
    for (int i = 1; i < digits; ++i)
        pow10 *= 10;

    return last * pow10 + rest;
}

/* Sequence cache – grows as needed */
static seq_t *cache = NULL;
static size_t cache_alloc = 0;   /* allocated array size (>= 2)       */
static size_t cache_computed = 1; /* highest index computed (>= 1)    */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1, but keep type */

    size_t idx = (size_t)n;          /* n fits into size_t on the tests */
    /* Initial allocation */
    if (cache == NULL) {
        cache_alloc = 2; /* indices 0 and 1 */
        cache = (seq_t *)calloc(cache_alloc, sizeof(seq_t));
        cache[1] = 1;    /* a(1) */
    }

    /* Grow allocation if necessary */
    if (idx >= cache_alloc) {
        size_t new_alloc = cache_alloc;
        while (new_alloc <= idx)
            new_alloc <<= 1;
        cache = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        /* newly grown slots are zero-initialised automatically (`realloc` may not) */
        cache_alloc = new_alloc;
    }

    /* Compute missing terms up to n */
    if (idx > cache_computed) {
        for (size_t i = cache_computed + 1; i <= idx; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = prev + rotate_digit_right(prev);
        }
        cache_computed = idx;
    }

    return cache[idx];
}
