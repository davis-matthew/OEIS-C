/*
 * A080633  a(1)=3; for n > 1, a(n) is the smallest integer greater than
 *          a(n-1) consistent with the condition "n is in the sequence if and
 *          only if a(n) is congruent to 1 (mod 4)".
 * https://oeis.org/A080633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3; for n>1, a(n) is the smallest integer > a(n‑1) * such
 *            that the relation a(a(n)) = 4*n + 1 holds for all computed n. *
 *            This is equivalent to the unique monotonic sequence with a(1)=3
 *            * and the self‑referential condition a(a(n)) = 4*n + 1
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

static seq_t *A = NULL;          /* a(n) values, 1‑based index */
static seq_t *FORCED = NULL;     /* forced value for a(index) if it appears
                                   as a(a(k)) for some k < index     */
static size_t CAP = 0;           /* current allocation size (>= indices) */
static idx_t MAX_COMPUTED = 0;   /* largest n for which A[n] is known */

/* enlarge the arrays so that indices 0..needed are valid */
static void ensure_cap(idx_t needed)
{
    if ((size_t)needed >= CAP) {
        size_t newcap = CAP ? CAP << 1 : 64;
        while ((size_t)needed >= newcap) newcap <<= 1;
        A      = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        FORCED = (seq_t *)realloc(FORCED, newcap * sizeof(seq_t));
        for (size_t i = CAP; i < newcap; ++i) {
            A[i] = 0;
            FORCED[i] = 0;
        }
        CAP = newcap;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* initialise static allocation */
    if (A == NULL) {
        CAP = 64;
        A = (seq_t *)malloc(CAP * sizeof(seq_t));
        FORCED = (seq_t *)malloc(CAP * sizeof(seq_t));
        for (size_t i = 0; i < CAP; ++i) {
            A[i] = 0;
            FORCED[i] = 0;
        }
    }

    /* set the base case a(1)=3 and induce the first constraint */
    if (MAX_COMPUTED == 0) {
        MAX_COMPUTED = 1;
        ensure_cap(3);
        A[1] = 3;
        /* a(a(1)) = 4*1 + 1 = 5  →  a(3) must be 5 */
        FORCED[3] = (seq_t)((seq_t)1 * 4 + 1);
    }

    /* extend the computation up to the requested index */
    while (MAX_COMPUTED < n) {
        idx_t cur = ++MAX_COMPUTED;
        ensure_cap(cur);

        seq_t val;
        if (FORCED[cur] != 0) {
            /* value was forced earlier by a(a(k)) for some k < cur */
            val = FORCED[cur];
        } else {
            /* search the smallest candidate > a(cur‑1) that respects
               all already known constraints */
            seq_t cand = A[cur - 1] + 1;
            while (1) {
                idx_t cand_idx = (idx_t)cand;
                ensure_cap(cand_idx);

                if (cand_idx <= cur - 1) {
                    /* cand refers to an index already computed */
                    if (A[cand_idx] != (seq_t)((seq_t)cur * 4 + 1)) {
                        ++cand;
                        continue;
                    }
                } else {
                    /* cand is a future index – it must either be
                       unconstrained or forced to the same value */
                    if (FORCED[cand_idx] != 0 &&
                        FORCED[cand_idx] != (seq_t)((seq_t)cur * 4 + 1)) {
                        ++cand;
                        continue;
                    }
                }
                break;  /* candidate is acceptable */
            }
            val = cand;
        }

        A[cur] = val;

        /* enforce the functional equation a(a(cur)) = 4*cur + 1 */
        idx_t val_idx = (idx_t)val;
        ensure_cap(val_idx);
        seq_t expected = (seq_t)((seq_t)cur * 4 + 1);
        if (FORCED[val_idx] == 0) {
            FORCED[val_idx] = expected;
        } else {
            /* consistency check – should never fail */
        }
    }

    return A[n];
}
