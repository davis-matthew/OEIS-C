/*
 * A051807  Numbers with nonzero multiplicative digital root 5.
 * https://oeis.org/A051807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051807 – Numbers with nonzero multiplicative digital root 5
   Algorithm:  iterate through natural numbers, compute the multiplicative
   digital root (MDR) by repeatedly multiplying its non‑zero decimal
   digits until a single digit remains, and collect those with MDR == 5.
   Results are memoised in a static cache to allow arbitrary calling order.
   Complexity is O(number of examined integers); the 10000th term is
   below 10⁶, well within the time limit. */
#include <stdlib.h>

/* Helper: compute the multiplicative digital root of a seq_t value.
   The routine works for any concrete type of seq_t (fixed‑size or
   arbitrary precision) because it uses only % , / , < , >=, +, *, and
   constants.  Zeros are ignored in the product. */
static seq_t mdr(seq_t x)
{
    seq_t t = x;
    while (t >= 10) {                     /* more than one digit? */
        seq_t prod = 1;
        seq_t y = t;
        while (y > 0) {                   /* process each digit */
            unsigned d = (unsigned)(y % 10);   /* digit 0–9 */
            if (d) prod *= d;          /* ignore zero digits */
            y /= 10;
        }
        t = prod;                        /* repeat until single digit */
    }
    return t;                             /* 0‑9, the MDR */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache for memorised terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap   = 0;

    /* current integer being tested (not stored yet) */
    static seq_t cur = 1;

    /* ensure the cache can hold at least one more entry */
    if (cache_size == cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 64;   /* initial 64 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        /* find next integer whose MDR is 5 */
        while (mdr(cur) != 5) {
            cur += 1;
        }
        /* store the found term */
        cache[cache_size++] = cur;
        /* advance to start search at next integer */
        cur += 1;
    }

    return cache[n - 1];
}
