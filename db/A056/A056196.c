/*
 * A056196  Numbers n such that A055229(n) = 2.
 * https://oeis.org/A056196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 5254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   valid_odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056196: numbers of the form 2^(2*k+1)*m, k≥1, m odd,
   and every prime divisor of m has exponent either 1 or an even
   number.  The function returns the n‑th such number (1‑indexed).  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/valid_odd_part.h"

/* cache of already generated terms */
static seq_t *terms      = NULL;  /* array of seq_t values                */
static size_t terms_cap  = 0;      /* allocated capacity of the array      */
static size_t terms_cnt  = 0;      /* number of terms stored              */

/* enlarge the terms cache if necessary */
static void ensure_terms_capacity(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (newp == NULL) exit(1);       /* allocation failure */
    terms      = newp;
    terms_cap  = new_cap;
}

/* append a new value to the cached sequence */
static void add_term(uint64_t val)
{
    ensure_terms_capacity(terms_cnt + 1);
    terms[terms_cnt++] = (seq_t)val;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset handling (not used) */

    /* return already computed term if available */
    if (terms_cnt >= (size_t)n) return terms[n - 1];

    uint64_t candidate = (terms_cnt > 0) ? (uint64_t)terms[terms_cnt - 1] + 1
                                        : 8;                  /* first term */

    while (terms_cnt < (size_t)n) {
        /* ensure the exponent of 2 is odd and at least 3 */
        uint64_t tmp = candidate;
        uint32_t exp2 = 0;
        while ((tmp & 1) == 0) {          /* count trailing zeros   */
            tmp >>= 1;
            ++exp2;
        }

        if ((exp2 >= 3) && (exp2 & 1)) {  /* odd exponent ≥3?     */
            uint64_t odd_part = tmp;      /* remainder after removing 2's */
            if (valid_odd_part(odd_part)) {
                add_term(candidate);
            }
        }
        ++candidate;
    }

    return terms[n - 1];
}
