/*
 * A130494  Row sums of triangle A130478.
 * https://oeis.org/A130494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A130494: Row sums of triangle A130478
   Formula: a(1) = 1; for n > 1,
   a(n) = n! + (n-1)! + 2*∑_{k=0}^{n-2} k! – 1
         = n! + (n-1)! + 2*prefix[n‑2] – 1
   where prefix[i] = ∑_{k=0}^{i} k!.  */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build.  */

static seq_t *fact     = NULL;   /* fact[i] = i!      */
static seq_t *prefix   = NULL;   /* prefix[i] = Σ_{k=0}^i k! */
static idx_t fact_max = 0;       /* highest index currently cached */

/* Computes the n-th term of the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Safety guard – not used in the sequence.  */
        return (seq_t)0;
    }

    /* Ensure caches contain values up to n.  */
    if (n > fact_max) {
        size_t new_cap = (size_t)(n + 1);                /* array capacity needed   */
        seq_t *f, *p;                                    /* temporary pointers      */

        if (fact == NULL) {
            /* First allocation.  */
            f = (seq_t*)malloc(new_cap * sizeof(seq_t));
            p = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (f == NULL || p == NULL) {
                /* Allocation failed – return 0 as a fallback.  */
                return (seq_t)0;
            }
            fact   = f;
            prefix = p;
            fact[0]   = (seq_t)1;            /* 0! = 1          */
            prefix[0] = (seq_t)1;            /* Σ_{k=0}^0 k! = 1*/
            fact_max = 0;
        } else {
            /* Grow the arrays.  */
            f = (seq_t*)realloc(fact, new_cap * sizeof(seq_t));
            p = (seq_t*)realloc(prefix, new_cap * sizeof(seq_t));
            if (f == NULL || p == NULL) {
                /* Allocation failed – return 0 as a fallback.  */
                return (seq_t)0;
            }
            fact   = f;
            prefix = p;
        }

        /* Compute new factorials and prefix sums.  */
        for (idx_t i = fact_max + 1; i <= n; ++i) {
            fact[i]   = fact[i - 1] * (seq_t)i;
            prefix[i] = prefix[i - 1] + fact[i];
        }
        fact_max = n;
    }

    /* a(1) = 1.  */
    if (n == 1) {
        return (seq_t)1;
    }

    /* For n > 1: a(n) = fact[n] + fact[n-1] + 2*prefix[n-2] - 1.  */
    seq_t result = fact[n] + fact[n - 1]
                 + (seq_t)2 * prefix[n - 2]
                 - (seq_t)1;
    return result;
}
