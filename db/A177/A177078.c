/*
 * A177078  Central polygonal-free numbers.
 * https://oeis.org/A177078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:
   Compute the n-th positive integer that is not divisible by any
   central polygonal number greater than one.
   Central polygonal numbers are given by c(j) = j(j+1)/2 + 1.
   All multiples of 2 are removed by skipping even candidates.
   We keep a dynamic list of odd central polygonal numbers (c ≥ 3)
   and test each odd candidate for divisibility by them. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t   *central  = NULL;   /* list of odd central polygonal numbers (≥3) */
static size_t  central_sz  = 0;    /* number of items in the list   */
static size_t  central_cap  = 0;    /* allocated capacity of the list */
static idx_t   j_next = 2;         /* next index j to generate c(j) */

/* Ensure that all odd central polygonal numbers ≤ limit are stored in the list. */
static void
ensure_central_upto(idx_t limit)
{
    while (1) {
        seq_t c = ((seq_t)j_next * (j_next + 1)) / 2 + 1;  /* c(j_next) */
        if (c > limit) break;

        /* Store only odd c (even ones are redundant because all candidates are odd). */
        if ( (c & (seq_t)1) != 0 ) {
            if (central_sz == central_cap) {
                central_cap = (central_cap == 0) ? 16 : central_cap * 2;
                central = (seq_t*)realloc(central, central_cap * sizeof(seq_t));
                if (!central) abort();  /* memory allocation failure */
            }
            central[central_sz++] = c;
        }
        ++j_next;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not defined for n < 1 */

    idx_t   count = 0;     /* how many valid numbers found so far */
    seq_t   val   = 1;     /* start just before the first odd candidate (3) */

    for (;;) {
        val += 2;                     /* consider only odd numbers */
        ensure_central_upto((idx_t)val);

        bool ok = true;
        for (size_t i = 0; i < central_sz; ++i) {
            seq_t c = central[i];
            if (c > val) break;
            if (val % c == 0) { ok = false; break; }
        }

        if (ok) {
            ++count;
            if (count == n) return val;
        }
    }
}
