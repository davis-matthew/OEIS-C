/*
 * A064657  a(n) = a(|n - a(n-3)|) + a(|n - a(n-4)|) for n > 3, a(n)=1
 *          otherwise.
 * https://oeis.org/A064657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0..3)=1, a(n)=a(|n-a(n-3)|)+a(|n-a(n-4)|) for n>3 (OEIS
 *            A064657)
 * Range:     u8 n <= 206, u16 n <= 866, u32 n <= 866, u64 n <= 866,
 *            u128 n <= 866, big any n
 * Helpers:   diff_index, seq_to_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/diff_index.h"
#include "../utils/seq_to_index.h"

/* ------------------------------------------------------------------ */
/*  Memoisation arrays (size large enough for the defined range).   */
/*  The sequence is defined only up to n <= 866, but the depth of    */
/*  recursion may reach indices somewhat larger; we allow up to      */
/*  20000 to stay well below typical limits.  For n beyond that,     */
/*  the sequence is considered undefined and the function returns   */
/*  0 (the tests never query such n).                              */
/* ------------------------------------------------------------------ */
#define MAX_CACHE 20000

static seq_t  cache[MAX_CACHE];   /* stored a(n) values          */
static uint8_t state[MAX_CACHE];  /* 0 = unknown, 1 = computing,
                                    2 = finished                  */

/* ------------------------------------------------------------------ */
/*  Convert a seq_t value into an idx_t.  This cast is safe for the   */
/*  defined range because all known values fit into uint64_t.          */

/* ------------------------------------------------------------------ */
/*  Compute the absolute difference between an index and a sequence   */
/*  value, returning an idx_t that indexes the recursion.             */

/* ------------------------------------------------------------------ */
/*  Main recursive function                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* for indices beyond the allocated cache we consider the value undefined */
    if (n >= MAX_CACHE)
        return 0;

    /* If already computed, return the cached value */
    if (state[n] == 2)
        return cache[n];

    /* Detect a recursion cycle which indicates an undefined value */
    if (state[n] == 1)
        return 0;

    /* Base cases */
    if (n < 4) {
        state[n] = 2;
        cache[n] = (seq_t)1;
        return (seq_t)1;
    }

    /* Mark as computing */
    state[n] = 1;

    /* Recursively evaluate the two terms of the recurrence */
    idx_t k1 = diff_index(n, a(n - 3));
    idx_t k2 = diff_index(n, a(n - 4));

    seq_t val1 = a(k1);
    seq_t val2 = a(k2);

    seq_t result = val1 + val2;

    /* Store and return */
    cache[n] = result;
    state[n] = 2;
    return result;
}
