/*
 * A045207  Numbers whose base-5 representation contains exactly one 0 and one
 *          4.
 * https://oeis.org/A045207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-5 representation contains exactly one 0 and one 4. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper to test whether a number and only has one digit 0
   and one digit 4 in its base‑5 representation. */
static bool is_valid(seq_t x)
{
    seq_t count0 = 0, count4 = 0;
    seq_t tmp = x;
    while (tmp > (seq_t)0) {
        seq_t d = tmp % (seq_t)5;
        if (d == (seq_t)0)
            count0++;
        else if (d == (seq_t)4)
            count4++;
        tmp = tmp / (seq_t)5;
    }
    return (count0 == (seq_t)1) && (count4 == (seq_t)1);
}

/* Return the n-th term (1‑based).  A static cache is used so
   that repeated calls are fast.  The cache grows as needed. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t candidate = 0;   /* last checked number */

    while (cache_len < n) {
        candidate++;                      /* next number to test */
        if (is_valid(candidate)) {
            if (cache_len >= cache_cap) { /* grow cache */
                size_t new_cap = (cache_cap == 0 ? 16 : cache_cap * 2);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;   /* store valid number */
        }
    }
    return cache[n - 1];
}
