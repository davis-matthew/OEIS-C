/*
 * A021756  Decimal expansion of 1/752.
 * https://oeis.org/A021756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A021756: a(n) is the nth digit after the decimal point in the
   decimal expansion of 1/752.  The digits are computed by the
   standard long‑division algorithm, so the sequence is exact whenever
   a(n) fits in the type seq_t. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static cache of computed digits */
    static seq_t *digits = NULL;        /* array of digits a(i) */
    static size_t cap = 0;              /* allocated capacity */
    static idx_t last = (idx_t)-1;      /* last computed index, -1 = none yet */
    static seq_t rem = 1;               /* remainder before generating the next digit */

    /* first use: allocate initial capacity */
    if (digits == NULL) {
        cap = 1;
        digits = (seq_t *)malloc(cap * sizeof(seq_t));
        if (digits == NULL) return 0;   /* allocation failure */
    }

    /* Grow capacity if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap > 0 ? cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;     /* allocation failure */
        digits = tmp;
        cap = new_cap;
    }

    /* Compute digits up to n (inclusive) */
    for (idx_t i = last + 1; i <= n; ++i) {
        rem *= 10;                     /* bring down next decimal digit */
        digits[i] = rem / (seq_t)752;   /* current digit of 1/752 */
        rem %= (seq_t)752;             /* new remainder for next step */
    }
    last = n;

    return digits[n];
}
