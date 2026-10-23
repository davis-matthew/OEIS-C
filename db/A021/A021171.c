/*
 * A021171  Decimal expansion of 1/167.
 * https://oeis.org/A021171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal expansion of 1/167:
   a(n) = the n‑th decimal digit after the point of 1/167
   (0‑based indexing, a(0)=0, a(1)=0, a(2)=5, …) */
#include <stdlib.h>

static seq_t *digits = NULL;        /* cached digits */
static idx_t  digits_size  = 0;     /* number of digits stored (next free index) */
static size_t digits_cap   = 0;     /* allocated capacity */
static uint64_t rem_after_last = 0; /* remainder after the last stored digit */

seq_t a(idx_t n) {
    /* Initialise storage and first digit on first call */
    if (digits_size == 0) {
        digits_cap = 1;
        digits = (seq_t *)malloc(digits_cap * sizeof(seq_t));
        if (!digits) return (seq_t)0;   /* allocation failure (shouldn't happen) */

        uint64_t rem = 1;                     /* 1/167 has remainder 1 */
        uint64_t tmp = rem * 10;              /* 10*rem */
        digits[0] = (seq_t)(tmp / 167);       /* first digit a(0) = 0 */
        rem_after_last = tmp % 167;           /* remainder for next digit */
        digits_size = 1;
    }

    if ((idx_t)n < digits_size) {
        /* digit already computed */
        return digits[n];
    }

    /* Ensure enough capacity for the requested index */
    if ((size_t)n >= digits_cap) {
        size_t new_cap = digits_cap;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        digits = tmp;
        digits_cap = new_cap;
    }

    /* Compute missing digits up to index n */
    uint64_t rem = rem_after_last;
    for (idx_t i = digits_size; i <= n; ++i) {
        uint64_t tmp = rem * 10;            /* remainder * 10 */
        digits[i] = (seq_t)(tmp / 167);     /* the digit a(i) */
        rem = tmp % 167;                    /* update remainder */
    }

    rem_after_last = rem;
    digits_size = n + 1;
    return digits[n];
}
