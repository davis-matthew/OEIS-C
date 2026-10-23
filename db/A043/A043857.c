/*
 * A043857  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 7 mod 8.
 * https://oeis.org/A043857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   count_base4_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n‑th number whose base‑4 representation
 * has a number of runs congruent to 7 modulo 8.
 *
 * The sequence starts with 4369, 4370, 4371, … as listed in OEIS
 * A043857.  The function uses a simple memoisation strategy:
 * already discovered terms are stored in a dynamic array of
 * idx_t.  When seq_t is an arbitrary‑precision integer class,
 * the stored idx_t value is converted to seq_t on return via a
 * constructor that accepts an unsigned long long.  The implementation
 * remains valid for both the C11 and the C++17 builds provided by
 * the check harness. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/count_base4_runs.h"

/* memoised storage: idx_t array holding the k values already found */
static idx_t  *terms   = NULL;
static size_t  term_cnt = 0;
static size_t  term_cap = 0;
static idx_t   next_k   = 4369;          /* first candidate to test */

/* ensure that space for at least one more term exists */
static void grow_if_needed(void)
{
    if (term_cnt >= term_cap) {
        size_t new_cap = term_cap == 0 ? 64 : term_cap << 1;
        idx_t *tmp = (idx_t *)realloc(terms, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        terms = tmp;
        term_cap = new_cap;
    }
}

/* append a discovered term */
static void add_term(idx_t k)
{
    grow_if_needed();
    terms[term_cnt++] = k;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined below offset – harmless */

    /* generate terms until we have n of them */
    while (term_cnt < n) {
        uint32_t runs = count_base4_runs((uint64_t)next_k);
        if ((runs & 7) == 7) add_term(next_k);  /* runs % 8 == 7 */
        ++next_k;
    }
    return (seq_t)terms[n - 1];
}
