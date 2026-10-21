/*
 * A227411  Palindromic prime numbers representing a date in "condensed
 *          European notation" DDMMYY.
 * https://oeis.org/A227411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, less
 * Formula:   a(n) is the nth palindromic prime number of the form DDMMYY
 *            representing a valid European date in condensed decimal
 *            notation. The sequence is finite (14 terms)
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   is_palindrome, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_palindrome.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Comparison function for qsort on seq_t values. */
static int cmp_seq(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;

    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Build the cache lazily on first use. */
static void build_cache(void)
{
    /* Days per month, February fixed to 28 days. */
    static const int days_in_month[13] = {
        0,  /* unused index 0 */
        31, /* Jan */
        28, /* Feb */
        31, /* Mar */
        30, /* Apr */
        31, /* May */
        30, /* Jun */
        31, /* Jul */
        31, /* Aug */
        30, /* Sep */
        31, /* Oct */
        30, /* Nov */
        31  /* Dec */
    };

    size_t cap = 16;  /* initial capacity for cache */
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cache) abort();  /* fatal allocation failure */

    size_t len = 0;  /* number of elements currently stored */

    for (int y = 0; y <= 99; ++y) {
        for (int m = 1; m <= 12; ++m) {
            int days = days_in_month[m];
            for (int d = 1; d <= days; ++d) {
                /* Build decimal value DDMMYY. */
                uint64_t tmp = (uint64_t)d * 10000ULL
                            + (uint64_t)m * 100ULL
                            + (uint64_t)y;
                seq_t val = (seq_t)tmp;

                if (is_palindrome(val, 10) && is_prime_big(val)) {
                    if (len == cap) {
                        cap <<= 1;
                        seq_t *p = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                        if (!p) { free(cache); abort(); }
                        cache = p;
                    }
                    cache[len++] = val;
                }
            }
        }
    }

    /* Sort the collected values to provide them in increasing order. */
    qsort(cache, len, sizeof(seq_t), cmp_seq);
    cache_len = len;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;          /* undefined for n = 0 */

    if (!cache) build_cache();                   /* lazily initialize */

    if (n <= (idx_t)cache_len) return cache[n - 1];
    return (seq_t)0;                             /* beyond finite sequence */
}
