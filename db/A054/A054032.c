/*
 * A054032  Numbers n such that n^2 contains exactly 4 different digits.
 * https://oeis.org/A054032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   compute the nth integer n such that n^2 contains exactly 4
 *            distinct decimal digits
 * Range:     u8 n <= 110, u16 n <= 3130, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_digits, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the sequence is defined by a property of n^2; no closed form is known,
   so the implementation uses a simple incremental search with memoisation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"    /* for grow_seq */
#include "../utils/count_distinct_digits.h"

static seq_t  *terms = NULL;     /* stored terms of the sequence   */
static size_t terms_len = 0;     /* current number of stored terms */
static size_t terms_cap = 0;     /* current allocated capacity   */
static uint64_t next_candidate = 1;   /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 0 – return 0 by convention */

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* search for the next candidate whose square has 4 distinct digits */
        while (1) {
            uint64_t cand = next_candidate++;
            unsigned __int128 sq = (unsigned __int128)cand * cand;
            if (count_distinct_digits(sq) == 4) {
                /* ensure capacity and store the found term */
                grow_seq(&terms, &terms_cap, terms_len + 1);
                terms[terms_len++] = (seq_t)cand;
                break;
            }
        }
    }
    return terms[n - 1];
}
