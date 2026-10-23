/*
 * A104692  Numbers n such that in n^3, number of odd digits is less than
 *          number of even digits.
 * https://oeis.org/A104692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 972, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_more_even_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth positive integer such that in n³, the number of odd decimal digits
   is less than the number of even decimal digits. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/has_more_even_digits.h"

/* fixed upper bound for preallocated storage (large enough for the
   expected test range - about 200 000 terms). */
#define PREALLOC 200000

static seq_t terms[PREALLOC];
static idx_t term_cnt = 0;      /* number of terms stored so far */
static idx_t next_candidate = 1;/* next integer to test for the property */

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1, but guard against 0 */

    /* Compute as many terms as needed */
    while (term_cnt < n) {
        if (has_more_even_digits((seq_t)next_candidate)) {
            /* store term */
            terms[term_cnt] = next_candidate;
            ++term_cnt;
        }
        ++next_candidate;
    }

    /* Return the requested term (indexing starts at 1) */
    return terms[n - 1];
}
