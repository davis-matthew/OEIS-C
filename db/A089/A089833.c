/*
 * A089833  a(n) = A000108(n)*(A000142(n+1)-1).
 * https://oeis.org/A089833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) * ((n+1)! - 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

/* Compute A089833: a(n) = Catalan(n) * ((n+1)! – 1)   */
// The function is memoised; it works for any seq_t type provided by the build.
seq_t a(idx_t n)
{
    /* Memoised factorial table */
    static seq_t *fact_cache = NULL;   /* fact_cache[i] = i!  */
    static size_t fact_cap   = 0;      /* allocated size in elements   */
    static idx_t  fact_max   = 0;      /* highest index calculated (i!) */

    /* We need (n+1)!  →  index n+1 in the table  */
    idx_t needed = n + 1;

    /* Allocate or grow the table if necessary */
    if (!fact_cache) {
        /* First allocation – store 0! = 1 */
        fact_cap = needed + 1;
        fact_cache = (seq_t *)malloc(fact_cap * sizeof(seq_t));
        if (!fact_cache) return (seq_t)0;      /* out of memory – unlikely */
        fact_cache[0] = (seq_t)1;
        fact_max   = 0;
    } else if (fact_cap <= needed) {
        /* Grow the table to hold the needed index */
        fact_cap = needed + 1;
        fact_cache = (seq_t *)realloc(fact_cache, fact_cap * sizeof(seq_t));
        if (!fact_cache) return (seq_t)0;
        /* newly allocated slots are uninitialised – we will fill them below */
    }

    /* Compute factorials up to the required index */
    for (idx_t i = fact_max + 1; i <= needed; ++i) {
        /* fact_cache[i] = i! = (i-1)! * i  */
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_max = needed;  /* now fact_cache[0..fact_max] are valid */

    /* Compute the desired term */
    seq_t cat      = catalan(n);          /* Catalan number C_n   */
    seq_t fact_n1  = fact_cache[n + 1];   /* (n+1)!              */
    seq_t tmp      = fact_n1 - (seq_t)1;  /* (n+1)! - 1          */

    return cat * tmp;                     /* a(n) = C_n * ( (n+1)! – 1 ) */
}
