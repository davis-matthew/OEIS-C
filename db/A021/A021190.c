/*
 * A021190  Decimal expansion of 1/186.
 * https://oeis.org/A021190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10^(n+1) / 186⌋ mod 10 (the nth digit after the decimal
 *            point of 1/186)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of already computed digits */
    static seq_t *digits = NULL;      /* array of digits 0..max_n */
    static idx_t max_n = (idx_t)-1;   /* last index computed */
    static size_t cap = 0;            /* allocated capacity */
    static unsigned long long rem = 1;/* remainder after last computed digit */

    /* Ensure we have computed up to index n */
    if ((idx_t)cap <= n) {
        /* Grow the array to size n+1 */
        size_t new_cap = (size_t)(n + 1);
        if (digits == NULL) {
            digits = seq_alloc(new_cap);
        } else {
            digits = seq_realloc(digits, cap, new_cap);
        }
        if (digits == NULL) {
            /* Allocation failed – fall back to an error value */
            return (seq_t)0;
        }
        cap = new_cap;
    }

    /* Compute missing digits if needed */
    for (idx_t i = (max_n == (idx_t)-1 ? 0 : max_n + 1); i <= n; ++i) {
        /* Long‑division step: multiply remainder by 10, extract digit */
        unsigned long long prod = rem * 10ULL;        /* fits in 64‑bit */
        unsigned long long digit = prod / 186ULL;     /* 0..9 */
        rem = prod % 186ULL;                         /* new remainder */
        digits[(size_t)i] = (seq_t)digit;            /* store digit */
    }
    max_n = n; /* update last computed index */

    return digits[(size_t)n];
}
