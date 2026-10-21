/*
 * A224756  a(2)=5; thereafter a(n) = smallest number m such that a(n-1)+m =
 *          (a(n-1) followed by the leading digit of m).
 * https://oeis.org/A224756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 5. For n > 1, let a(n) be the smallest integer m such
 *            that a(n) = a(n−1) + m = 10·a(n−1) + d, where d is the leading
 *            digit of m. Equivalently: m = 9·a(n−1) + d and the leading digit
 *            of m equals d
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 26,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Needed standard headers */
#include <stdlib.h>
#include <stdbool.h>

/* Utility headers for arbitrary‑precision and decimal digit helpers */
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Forward declaration of helper to get the leading decimal digit */
static seq_t leading_digit(seq_t x);

/* Sequence function */
seq_t a(idx_t n)
{
    /* Ensure the first term is available */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_index = 0;

    /* On first call, allocate minimal storage and set a(1) */
    if (cache == NULL) {
        cache_size = 2;                                    /* indices 0..1 */
        cache     = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(1);
        cache[1] = (seq_t)5;                               /* a(1) = 5 */
        max_index = 1;
    }

    /* Grow allocation if necessary */
    if (n >= cache_size) {
        size_t new_size = n + 1;                            /* indices up to n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute consecutive terms until we reach n */
    for (idx_t i = max_index + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t cand;
        bool found = false;

        for (int d = 1; d <= 9 && !found; ++d) {
            cand = prev * (seq_t)9 + (seq_t)d;              /* m = 9*a_prev + d */
            if (leading_digit(cand) == (seq_t)d) {
                cache[i] = cand;
                found = true;
            }
        }

        /* The construction guarantees a value will be found.
           If not, use the last candidate as a fallback (shouldn't happen). */
        if (!found) cache[i] = cand;
    }

    max_index = n;
    return cache[n];
}

/* Return the leading decimal digit of x */
static seq_t leading_digit(seq_t x)
{
    int len = digit_len(x);                                 /* number of digits */
    seq_t pow = pow10((idx_t)(len - 1));                     /* 10^(len-1) */
    return x / pow;                                         /* leading digit */
}
