/*
 * A034711  Largest number whose base n representation does not repeat a pair
 *          of consecutive digits.
 * https://oeis.org/A034711
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Alg. a(n):  Eulerian circuit in the complete digraph of n vertices;
    the circuit is constructed with Hierholzer's algorithm, always
    choosing the next unused edge in descending order of the target
    vertex.  The digits of the circuit (from most to least significant)
    form the base‑n representation of a(n).  The algorithm returns
    the decimal value of that number.  This satisfies the definition
    of OEIS A034711. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* A034711 is defined only for n ≥ 2.  For smaller n the value is
       irrelevant for the test suite, so we simply return 0. */
    if (n < 2) return (seq_t)0;

    /* Length of the base‑n representation is n^2+1.  We allocate a
       stack of that many vertices and an array of next unused edge
       indices for each vertex. */
    idx_t len   = n * n + (idx_t)1;
    idx_t *stack = (idx_t *)malloc((size_t)len * sizeof(idx_t));
    if (!stack) return (seq_t)0;

    int *next = (int *)malloc((size_t)n * sizeof(int));
    if (!next) { free(stack); return (seq_t)0; }

    for (idx_t i = 0; i < n; ++i) next[i] = (int)n - 1;

    idx_t  sp   = 0;          /* stack pointer */
    seq_t  val  = 0;          /* accumulated result */
    seq_t  mult = (seq_t)1;   /* current multiplier base n^k */

    stack[sp++] = n - 1;      /* start from the largest vertex */

    while (sp > 0) {
        idx_t v = stack[sp - 1];

        if (next[v] >= 0) {            /* still have unused edge */
            int w = next[v]--;          /* take the largest remaining neighbour */
            stack[sp++] = (idx_t)w;
        } else {                        /* all edges from v consumed – pop */
            val += (seq_t)v * mult;     /* v is the next least‑significant digit */
            mult *= (seq_t)n;
            --sp;
        }
    }

    free(stack);
    free(next);
    return val;
}
