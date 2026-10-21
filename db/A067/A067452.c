/*
 * A067452  Numbers, not composed of the same digits, such that the geometric
 *          and arithmetic means of their decimal digits are integers.
 * https://oeis.org/A067452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   seq_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A067452 by brute‐force search of integers.
   A number n is accepted if:
   – n has at least two digits,
   – not all of its decimal digits are equal,
   – none of its digits is 0,
   – the arithmetic mean of its digits is an integer,
   – the product of its digits is a perfect k‑th power
     (k = number of digits), i.e. it equals d^k for some integer
     1 ≤ d ≤ 9. */

#include <stdbool.h>
#include <stddef.h>

#include "../utils/seq_pow.h"
#include "../utils/seq_realloc.h"

/* Test whether n qualifies for the sequence */
static bool qualifies(uint64_t n)
{
    uint64_t tmp = n;
    int k = 0;                /* number of digits */
    int first_digit = -1;     /* first digit seen */
    int all_same = 1;         /* true if all digits seen so far are equal */
    uint64_t sum = 0;         /* sum of digits */
    seq_t prod = 1;           /* product of digits */

    while (tmp) {
        int d = (int)(tmp % 10ULL);
        if (d == 0)           /* zero digit disqualifies */
            return false;
        if (first_digit == -1)
            first_digit = d;
        else if (d != first_digit)
            all_same = 0;
        ++k;
        sum += (uint64_t)d;
        prod = prod * (seq_t)d;
        tmp /= 10ULL;
    }

    if (all_same)   /* all digits identical */
        return false;
    if (sum % (uint64_t)k != 0)  /* arithmetic mean not integer */
        return false;

    /* check if prod == d^k for some d in [1,9] */
    for (int d = 1; d <= 9; ++d) {
        seq_t power = seq_pow((seq_t)d, (idx_t)k);
        if (power == prod)
            return true;
    }
    return false;
}

/* Main sequence function.  The cache allows random access to
   previously computed terms.  The function is thread‑unsafe but
   satisfies the requirements. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* dynamic array storing a(1) .. a(cache_len) */
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t last_checked = 9;   /* last integer examined (>=9) */

    /* Return cached value when available */
    if (n > 0 && (size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Find new terms until we have at least n of them */
    for (;;) {
        ++last_checked;
        if (qualifies(last_checked)) {
            /* Grow cache if necessary */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 2 : cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
        if (cache_len >= (size_t)n)
            break;
    }
    return cache[(size_t)n - 1];
}
