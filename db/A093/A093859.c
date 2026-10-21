/*
 * A093859  Numbers whose sum of the digital products of the divisors is
 *          prime.
 * https://oeis.org/A093859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   digit_product, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A093859 (numbers whose sum of the digital products
 * of the divisors is prime).  The function stores found terms in a lazily‑
 * grown static cache so that subsequent calls may request terms in any
 * order. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"

/* Static cache of found terms */
static seq_t *cache   = NULL;   /* array of the found terms */
static size_t len     = 0;      /* number of stored terms */
static size_t cap     = 0;      /* allocated capacity */
static idx_t next     = 1;      /* next integer to test */

/* Grow the cache so it can hold at least 'need' elements. */
static void ensure_cap(size_t need)
{
    if (need <= cap)
        return;

    size_t new_cap = cap ? (cap << 1) : 8;
    if (new_cap < need)
        new_cap = need;

    /* Re‑allocate memory.  realloc handles NULL pointer case. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* Allocation failed – aborting.  In the contest environment the
         * program will be terminated, which is acceptable. */
        abort();
    }
    cache = tmp;
    cap   = new_cap;
}

/* Return the nth term of A093859 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return 0;           /* undefined behaviour for n = 0 */

    /* Generate terms until we have at least n of them. */
    while (len < (size_t)n) {
        const idx_t m = next;
        seq_t sum = 0;

        /* Enumerate all divisors of m. */
        for (idx_t d = 1; d <= m / d; ++d) {
            if (m % d == 0) {
                sum += digit_product((seq_t)d);
                idx_t d2 = m / d;
                if (d2 != d)
                    sum += digit_product((seq_t)d2);
            }
        }

        if (is_prime_big(sum)) {
            ensure_cap(len + 1);
            cache[len++] = (seq_t)m;
        }
        ++next;
    }

    return cache[(size_t)n - 1];
}
