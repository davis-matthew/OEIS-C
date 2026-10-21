/*
 * A051023  Middle column of rule-30 1-D cellular automaton, from a lone 1
 *          cell.
 * https://oeis.org/A051023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8217, u16 n <= 8217, u32 n <= 8217, u64 n <= 8217,
 *            u128 n <= 8217, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the middle column of cellular automaton rule‑30
   with a single live cell.  Simulation is performed iteratively
   for each call, using only standard C operations. */

/* #include <stdio.h>  // not required for the function implementation */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    /* length needed for all rows up to step n:
       at step s the row length is 2*s+1,  
       so at step n we need indices 0 .. 2*n+1.
       Allocate a little extra to avoid bounds checks. */
    size_t len = (size_t)2 * n + 3;            /* 2*n+3 bytes */

    /* Allocate two byte arrays and zero‑initialize them. */
    uint8_t *cur   = (uint8_t *)calloc(len, 1);
    uint8_t *next  = (uint8_t *)calloc(len, 1);

    /* Place the initial single live cell in the centre (index n). */
    cur[n] = 1;

    /* Iterate n steps of rule‑30.  Rule30 next cell = left ⊕ (center ∨ right). */
    for (idx_t step = 0; step < n; ++step) {
        /* Compute all positions that can be non‑zero after this step.
           The highest relevant index is 2*n+1, which is guaranteed ≤ len‑2. */
        for (size_t i = 0; i <= len - 2; ++i) {
            uint8_t left   = (i > 0)   ? cur[i - 1] : 0;
            uint8_t center = cur[i];
            uint8_t right  = (i + 1 < len) ? cur[i + 1] : 0;
            next[i] = left ^ (center | right);
        }
        /* Swap the buffers for the next iteration. */
        uint8_t *tmp = cur; cur = next; next = tmp;
    }

    uint8_t result = cur[n];   /* middle column after n steps */
    free(cur);
    free(next);
    return (seq_t)result;
}
