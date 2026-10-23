/*
 * A021170  Decimal expansion of 1/166.
 * https://oeis.org/A021170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   digits of decimal expansion of 1/166 computed via successive
 *            remainder iteration
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *digits   = NULL;   /* cached digits */
    static idx_t  count    = 0;      /* number of digits cached (next index to compute) */
    static idx_t  capacity = 0;      /* allocated capacity of 'digits' */
    static seq_t  remainder = 1;     /* current remainder before computing next digit */

    /* Initialise cache on first invocation */
    if (digits == NULL) {
        capacity  = 16;              /* start with a modest capacity */
        digits    = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (digits == NULL) {        /* out‑of‑memory – return 0 as a safe fallback */
            return 0;
        }
    }

    /* Return cached value if already known */
    if (n < count) {
        return digits[n];
    }

    /* Compute missing digits up to index n */
    while (count <= n) {
        /* Compute next digit using 10*rem div 166 */
        seq_t r10   = remainder * 10;
        seq_t digit = r10 / 166;
        remainder  = r10 % 166;

        /* Grow the cache if necessary */
        if (count == capacity) {
            size_t new_cap = capacity ? capacity * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (!tmp) {              /* allocation failure – return 0 */
                return 0;
            }
            digits = tmp;
            capacity = (idx_t)new_cap;
        }

        digits[count] = digit;
        ++count;
    }

    return digits[n];
}
