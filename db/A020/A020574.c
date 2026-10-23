/*
 * A020574  Smallest nonempty set S containing prime divisors of k + 10 for
 *          each k in S.
 * https://oeis.org/A020574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   cmp_seq_t, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal closed set under the mapping p → prime divisors of p+10.
   The sequence enumerates this set in increasing order.  The set is
   {3, 7, 11, 13, 17, 23}.  Values beyond the 6th term are undefined
   and the function returns 0. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/cmp_seq_t.h"    /* int cmp_seq_t(const void *p1, const void *p2); */

static seq_t *closed_set = NULL;   /* sorted sequence of primes in the minimal set */
static size_t set_len   = 0u;       /* number of elements in closed_set */

/* Initialise the closed set (once).  The set is small so a single
   allocation is enough, but we allocate dynamically for generality. */
static void
init_closed_set(void)
{
    if (closed_set != NULL) return;   /* already initialised */

    /* start with p = 3, the smallest prime that can be in a closed set */
    size_t cap     = 8u;
    closed_set     = (seq_t *)malloc(cap * sizeof(seq_t));
    set_len        = 0u;
    closed_set[set_len++] = (seq_t)3;

    /* process elements until no new primes are added */
    size_t processed = 0u;
    while (processed < set_len) {
        seq_t p = closed_set[processed++];
        seq_t q = p + (seq_t)10;              /* q = p + 10 */

        /* factor q to get its distinct prime factors */
        uint64_t primes[32];
        uint64_t exps[32];
        size_t   cnt = 0u;
        factor_u64((uint64_t)q, primes, exps, &cnt);

        for (size_t i = 0u; i < cnt; ++i) {
            seq_t r = (seq_t)primes[i];
            /* check whether r is already present in the set */
            bool present = false;
            for (size_t j = 0u; j < set_len; ++j)
                if (closed_set[j] == r) { present = true; break; }
            if (!present) {
                if (set_len == cap) {
                    cap *= 2;
                    closed_set = (seq_t *)realloc(closed_set, cap * sizeof(seq_t));
                }
                closed_set[set_len++] = r;
            }
        }
    }

    /* sort the set increasingly */
    qsort(closed_set, set_len, sizeof(seq_t), cmp_seq_t);
}

/* Return the nth element of the sequence (1‑based index).  If n is out
   of bounds, return 0. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;

    init_closed_set();                 /* ensure the set is computed */

    if ((idx_t) n > (idx_t) set_len) return (seq_t)0;
    return closed_set[(size_t)n - 1u];
}
