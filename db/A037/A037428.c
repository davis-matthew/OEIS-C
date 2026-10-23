/*
 * A037428  Positive numbers having the same set of digits in base 4 and base
 *          10.
 * https://oeis.org/A037428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 188, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A037428 – Positive numbers having the same set of digits in base 4 and base 10.
    a(n) is the nth integer x (n ≥ 1) such that the set of decimal digits of x equals the
    set of base‑4 digits of x. */
/*  Implementation: iteratively search integers, collect those satisfying the
    condition, and cache them for fast future look‑ups. */

#include <stdlib.h>
#include <stdbool.h>

/* Check whether a positive integer has the same set of digits in
   decimal (base 10) and in base 4.  Returns true only if all decimal
   digits are ≤ 3 and the digit sets coincide. */
static bool same_digit_set(idx_t x)
{
    unsigned int mask_dec = 0, mask_b4 = 0;
    idx_t tmp = x;

    /* Empty number (x == 0) is not considered positive; return false */
    if (tmp == 0) return false;

    /* Process decimal digits */
    while (tmp > 0) {
        unsigned d = tmp % 10;
        if (d > 3) return false;        /* digit not allowed in base 4 */
        mask_dec |= 1u << d;
        tmp /= 10;
    }

    /* Process base‑4 digits */
    tmp = x;
    while (tmp > 0) {
        unsigned d = tmp % 4;
        mask_b4 |= 1u << d;
        tmp /= 4;
    }

    return mask_dec == mask_b4;
}

/* Return the n‑th term of the sequence.  The sequence terms are
   cached so that multiple calls are efficient, regardless of call
   order. */
seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of stored terms */

    /* current starting point for the search */
    static idx_t current = 1;

    /* Expand cache until we have at least n terms */
    while (cache_len < n) {
        if (same_digit_set(current)) {
            /* Ensure space in cache */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? 2 * cache_cap : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* In a real program we would handle the error. */
                    exit(1);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)current;
        }
        ++current;
    }

    return cache[n - 1];
}
