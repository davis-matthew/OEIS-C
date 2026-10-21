/*
 * A070165  Irregular triangle read by rows giving trajectory of n in Collatz
 *          problem.
 * https://oeis.org/A070165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 314, u16 n <= 9416, u32 n <= 9416, u64 n <= 9416,
 *            u128 n <= 9416, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenation of Collatz trajectories for 1, 2, 3, … .
 * a(n) is the n‑th element of the sequence obtained by
 * listing for each integer i the values of the iterates of
 * T(i) (T(x)=x/2 if x even, 3x+1 if x odd) until the
 * value 1 is reached, with the rows concatenated in order. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {           /* safety check – the offset is 1 */
        return (seq_t)0;
    }

    /* static state – cache of generated elements
     * and index of the next row to process           */
    static seq_t *cache = NULL;    /* dynamic array of previously generated terms */
    static idx_t len   = 0;        /* number of elements currently in cache   */
    static idx_t cap   = 0;        /* current capacity of cache              */
    static idx_t next  = 1;        /* next integer whose Collatz trajectory
                                     * will be appended */
    /* Generate rows until we have at least n terms */
    while (len < n) {
        seq_t x = (seq_t)next;             /* start of the new row   */
        do {
            /* grow cache if required */
            if (len == cap) {
                size_t new_cap = (cap == (idx_t)0) ? (size_t)8 : (size_t)cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);   /* abort on allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = x;                /* store current value   */
            if (x == (seq_t)1) {             /* last element of the row */
                break;
            }                                 /* otherwise advance x   */
            if ((x & (seq_t)1) == (seq_t)0)   /* even: divide by 2      */
                x >>= 1;
            else                              /* odd: 3x+1             */
                x = x * (seq_t)3 + (seq_t)1;
        } while (1);
        ++next;                               /* prepare for next integer */
    }
    return cache[n - 1];                      /* one‑based index access */
}
