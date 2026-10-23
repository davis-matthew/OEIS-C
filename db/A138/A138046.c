/*
 * A138046  Positive integers k such that (d(k+1) - d(k)) * (-1)^k is
 *          positive, where d(k) = the number of positive divisors of k.
 * https://oeis.org/A138046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 9653, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A138046: Positive integers k such that (d(k+1)-d(k)) * (-1)^k > 0
 *  where d(n) = number of positive divisors of n.
 *  The first few terms are 45, 62, 74, 81, 105, …
 *
 *  Implementation: sieve divisor counts up to a limit that is doubled
 *  until at least n terms have been collected.  The sequence values are
 *  stored in a static array and indexed 1‑based.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>          /* for uint16_t */

/* static state for the sieve and collected terms */
static idx_t     _A138046_limit = 2000000;  /* initial maximum k to sieve */
static uint16_t *divcnt = NULL;             /* divisors count array      */
static seq_t   *terms  = NULL;             /* list of qualifying k      */
static idx_t    term_cnt = 0;              /* number of terms found     */
static bool     initialized = false;       /* did we build the tables?  */

/* build divisors counts up to 'limit' (inclusive) and collect terms   */
static void _build_terms(idx_t limit)
{
    size_t i, j;

    /* allocate divisor count array (need indices up to limit+1)    */
    if (divcnt) {
        free(divcnt);
    }
    divcnt = (uint16_t *)calloc((size_t)limit + 2, sizeof(uint16_t));
    if (!divcnt) {
        exit(1);   /* out of memory */
    }

    /* simple O(n log n) sieve for divisor counts                       */
    for (i = 1; i <= (size_t)(limit + 1); ++i) {
        for (j = i; j <= (size_t)(limit + 1); j += i) {
            ++divcnt[j];
        }
    }

    /* allocate terms array (worst case: all k up to limit are valid)   */
    if (terms) {
        free(terms);
    }
    terms = (seq_t *)malloc((size_t)limit * sizeof(seq_t));
    if (!terms) {
        exit(1);
    }

    /* collect all k satisfying the condition                          */
    term_cnt = 0;
    for (i = 1; i <= (size_t)limit; ++i) {
        int diff = (int)divcnt[i + 1] - (int)divcnt[i];
        if ((diff > 0 && ((i & 1U) == 0))   /* k even, need + difference */
            || (diff < 0 && ((i & 1U) != 0))) { /* k odd, need - difference */
            terms[term_cnt++] = (seq_t)i;
        }
    }
}

/* return the n‑th term of sequence A138046 (n is 1‑based)             */
seq_t a(idx_t n)
{
    /* build enough terms if necessary                                */
    if (!initialized || n > term_cnt) {
        while (n > term_cnt) {
            /* double the sieve range until enough terms are found      */
            _A138046_limit = (_A138046_limit == 0) ? 4096
                                                    : _A138046_limit * 2;
            _build_terms(_A138046_limit);
        }
        initialized = true;
    }

    /* return the requested term (0‑based index)                       */
    return terms[(size_t)n - 1];
}
