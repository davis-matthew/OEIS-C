/*
 * A076355  Numbers n such that 210*n + 11 is prime.
 * https://oeis.org/A076355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076355.
   a(n) = the nth integer m such that 210*m + 11 is prime.
   For a(1)=0, a(2)=2, a(3)=3, a(4)=5, ... */

/* The function may be called with any n>=1 in any order.
   It memoises found terms and searches for new ones only when needed. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* Static storage for memoised terms */
static seq_t *terms      = NULL;   /* array of m values      */
static size_t terms_cap  = 0;      /* allocated capacity     */
static size_t terms_cnt  = 0;      /* number of stored terms */

/* The first candidate that has not yet been tested.
   Updated after each successful test. */
static idx_t checked = 0;

/* Ensure the terms array has room for one more element. */
static void ensure_capacity(void)
{
    if (terms_cnt >= terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 16 : terms_cap << 1;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* allocation failed */
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)             /* undefined for n < 1; return 0 for safety */
        return 0;

    /* Return memoised value if already computed. */
    if ((size_t)n <= terms_cnt)
        return terms[(size_t)n - 1];

    /* Search for new terms until we reach the desired index. */
    for (idx_t cand = checked; ; ++cand) {
        idx_t candidate_prime = 210 * cand + 11;
        if (is_prime(candidate_prime)) {          /* cand satisfies condition */
            ensure_capacity();
            terms[terms_cnt] = (seq_t)cand;
            terms_cnt++;
            if (terms_cnt == (size_t)n) {         /* desired term found */
                checked = cand + 1;               /* next search starts after this */
                return cand;
            }
        }
        checked = cand + 1;                       /* update progress marker */
    }
}
