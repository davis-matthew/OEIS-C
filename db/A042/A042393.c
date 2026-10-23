/*
 * A042393  Denominators of continued fraction convergents to sqrt(723).
 * https://oeis.org/A042393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS sequence A042393 – Denominators of convergents to sqrt(723)
 *
 *  The sequence satisfies the linear recurrence
 *      a(0) = 1,   a(1) = 1
 *      a(n) = c[n] * a(n‑1) + a(n‑2)   for n ≥ 2
 *
 *  where the coefficient sequence c[n] repeats with period 4:
 *      c[1] = 1,   c[2] = 7,   c[3] = 1,   c[4] = 52
 *
 *  This implementation memoises computed terms so that calls
 *  are efficient even when requested in any order.
 */

#include <stdlib.h>

static seq_t *den = NULL;   /* cached denominators          */
static idx_t  max_n = 0;    /* highest index cached so far  */
static size_t capacity = 0; /* current allocation size      */

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* initialise the cache on first use */
    if (den == NULL) {
        capacity = 2;
        den = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!den) return 0;                 /* allocation failure */
        den[0] = (seq_t)1;
        den[1] = (seq_t)1;
        max_n  = 1;
    }

    /* extend the cache up to index n */
    while (max_n < n) {
        /* grow the array if necessary */
        if (capacity <= max_n + 1) {
            size_t new_cap = capacity ? capacity * 2 : 2;
            if (new_cap <= max_n + 1) new_cap = max_n + 2;
            seq_t *tmp = (seq_t *)realloc(den, new_cap * sizeof(seq_t));
            if (!tmp) return 0;             /* allocation failure */
            den = tmp;
            capacity = new_cap;
        }

        idx_t k = max_n + 1;           /* index of the new term   */
        seq_t coeff;
        switch ((k - 1) % 4) {         /* periodic coefficient   */
            case 0: coeff = (seq_t)1;  break;
            case 1: coeff = (seq_t)7;  break;
            case 2: coeff = (seq_t)1;  break;
            default: coeff = (seq_t)52; break;
        }

        den[k] = coeff * den[k - 1] + den[k - 2];
        max_n = k;
    }

    return den[n];
}
