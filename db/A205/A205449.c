/*
 * A205449  Least h such that n divides the h-th difference between distinct
 *          even-indexed Fibonacci numbers; the differences are ordered as in
 *          A205448.
 * https://oeis.org/A205449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least h such that n divides the h-th difference between
 *            distinct even-indexed Fibonacci numbers, where the differences
 *            are ordered by increasing larger index and then increasing
 *            smaller index
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 58, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, but safe fallback */

    /* allocate array to remember the first occurrence of each residue modulo n   */
    idx_t sentinel = n;                       /* value not used as an index (0 <= index < n)   */
    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!first) return 0;                     /* allocation failure – unlikely in tests    */
    for (idx_t i = 0; i < n; ++i) first[i] = sentinel;

    /* Fibonacci generation */
    seq_t f0 = 0;                         /* F_0   */
    seq_t f1 = 1;                         /* F_1   */
    idx_t fib_idx = 1;                    /* current index of f1 (currently F_1)     */
    idx_t even_idx = (idx_t)-1;           /* index of the last processed even‑indexed
                                            Fibonacci number; unset initially */

    while (1) {
        /* next Fibonacci number */
        seq_t fnext = f0 + f1;
        f0 = f1;
        f1 = fnext;
        ++fib_idx;                        /* now f1 is F_fib_idx                        */

        /* process only even indices (F_2, F_4, …) */
        if ((fib_idx & 1) == 0) {         /* even index */
            if (even_idx == (idx_t)-1) even_idx = 0;
            else                         even_idx++;

            idx_t residue = (idx_t)(f1 % n);    /* f1 == F_fib_idx */
            if (first[residue] != sentinel) {   /* we have seen this residue before   */
                idx_t i = first[residue];       /* earlier index with same residue    */
                seq_t h = (seq_t)even_idx *
                           (seq_t)(even_idx - 1) / 2 + (seq_t)(i + 1);
                free(first);
                return h;                       /* minimal h found                      */
            }
            /* first time this residue appears */
            first[residue] = even_idx;
        }
    }
}
