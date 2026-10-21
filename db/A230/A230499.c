/*
 * A230499  a(n) is the maximal number k of consecutive numbers of the form
 *          (2*n-1)*(2*i-1), i=1,2,...,k, which are all evil or all odious
 *          (A000069, A001969).
 * https://oeis.org/A230499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 2722, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   parity_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute OEIS sequence A230499
 *  a(n) is the maximal number of consecutive odd multiples of (2*n-1)
 *  that are all evil or all odious (same parity of popcount).
 *  (Odd numbers only, so the product fits comfortably in seq_t.)
 */

#include <stdbool.h>
#include "../utils/parity_seq_t.h"

seq_t a(idx_t n)
{
    if (n < 1)   /* offset is 1; caller should never ask for n<1 */
        return (seq_t)0;

    seq_t t = (seq_t)(2 * n - 1);            /* the fixed odd factor */

    /* Start with i = 1 (factor = 1) */
    seq_t prod = t * (seq_t)1;
    unsigned parity_ref = parity_seq_t(prod);

    unsigned k = 1;                          /* count already includes i=1 */

    for (idx_t i = 2;; ++i) {
        seq_t mult = (seq_t)(2 * i - 1);
        prod = t * mult;
        unsigned parity_cur = parity_seq_t(prod);
        if (parity_cur != parity_ref)
            break;                           /* parity changed – stop */
        ++k;
    }

    return (seq_t)k;
}
