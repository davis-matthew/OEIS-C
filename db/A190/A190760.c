/*
 * A190760  Product of digits is divisible by number of digits.
 * https://oeis.org/A190760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 173, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len_u, digit_product_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A190760: numbers whose product of decimal digits is divisible by
 * the number of digits.  The function scans natural numbers until
 * it has cached the requested n‑th element.  No closed‑form formula
 * exists for this sequence. */
#include <stdlib.h>
#include "../utils/digit_len_u.h"
#include "../utils/digit_product_u.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset guard, not used in OEIS */

    static seq_t *cache = NULL;    /* cache of accepted terms (1‑based) */
    static size_t cache_cap = 0;   /* allocated capacity of cache[]    */
    static size_t current_index = 0; /* number of stored terms (max i)  */
    static seq_t next_to_test = 0;    /* next natural number to examine */

    while (current_index < n) {
        seq_t prod = digit_product_u(next_to_test);
        unsigned d = digit_len_u(next_to_test);
        if (prod % (seq_t)d == 0) {
            /* Ensure enough space */
            if (current_index + 1 >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!p) exit(1);          /* out of memory */
                cache = p;
                cache_cap = new_cap;
            }
            cache[++current_index] = next_to_test;
        }
        ++next_to_test;
    }

    return cache[n];
}
