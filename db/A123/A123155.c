/*
 * A123155  From a quiz by Albert Frank.
 * https://oeis.org/A123155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A123155 (Albert Frank quiz). 
   a(1) = 2614534.  For n > 1, let S be the decimal digits of a(n‑1) (most‑significant first).
   For each distinct digit d that occurs in S, let i be its first (1‑based) position.
   Let v be the digit that follows i in S (cyclically).  Then a(n) is formed by writing
   v at position d (positions not indexed by any digit are omitted, i.e., treated as 0). */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* uint8_t */

static seq_t next_term(seq_t prev)
{
    /* Extract decimal digits of prev into an array in most‑significant order. */
    seq_t tmp = prev;
    size_t len = 0;
    do {
        tmp /= 10;
        len++;
    } while (tmp != 0);

    uint8_t *digits = (uint8_t *)malloc(len);
    if (!digits)
        return (seq_t)0; /* allocation failure, return 0 */

    tmp = prev;
    for (size_t i = 0; i < len; ++i) {
        seq_t d = tmp % 10;
        digits[len - 1 - i] = (uint8_t)d;
        tmp /= 10;
    }

    /* Find first occurrence of each digit 0..9.  Use -1 for not seen. */
    int first_pos[10];
    int seen[10];
    for (int d = 0; d < 10; ++d) {
        first_pos[d] = -1;
        seen[d] = 0;
    }
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = digits[i];
        if (!seen[d]) {
            seen[d] = 1;
            first_pos[d] = (int)i;   /* zero‑based index */
        }
    }

    /* Determine maximum digit that appears (positions are 1‑based). */
    int max_digit = -1;
    for (int d = 1; d <= 9; ++d)
        if (seen[d] && d > max_digit)
            max_digit = d;

    if (max_digit == -1) {
        free(digits);
        return (seq_t)0; /* no digits 1‑9 present */
    }

    /* Build new digits array: next_digits[d] = following digit of first d. */
    uint8_t next_digits[10];
    for (int i = 0; i <= max_digit; ++i)
        next_digits[i] = 0;   /* default 0 for omitted positions */

    for (int d = 1; d <= 9; ++d) {
        if (!seen[d]) continue;
        int i = first_pos[d];
        uint8_t v = digits[(i + 1) % len];
        next_digits[d] = v;
    }

    /* Assemble the result integer in decimal. */
    seq_t result = 0;
    for (int pos = 1; pos <= max_digit; ++pos)
        result = result * 10 + (seq_t)next_digits[pos];

    free(digits);
    return result;
}

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static const seq_t BASE = (seq_t)2614534;

    if (n < (idx_t)1)
        return (seq_t)0;      /* undefined for n < 1 */

    if (cache_len < (size_t)n) {
        /* enlarge cache if needed */
        size_t new_cap = cache_cap;
        if (new_cap == 0)
            new_cap = 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;     /* double until big enough */

        if (new_cap > cache_cap) {
            seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!ptr)
                return (seq_t)0;   /* allocation failure */
            cache = ptr;
            cache_cap = new_cap;
        }

        if (cache_len == 0) {
            cache[0] = BASE;
            cache_len = 1;
        }

        /* compute missing terms up to n */
        for (size_t idx = cache_len; idx < (size_t)n; ++idx)
            cache[idx] = next_term(cache[idx - 1]);

        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
