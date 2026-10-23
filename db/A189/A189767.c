/*
 * A189767  Least number k such that the set of numbers {Fibonacci(i) mod n,
 *          i=0..k-1} contains all possible residues of Fibonacci(i) mod n.
 * https://oeis.org/A189767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = least k such that the set {Fib(i) mod n | i = 0..k‑1}  
 *  contains all residues that appear in the Fibonacci sequence modulo n. 
 *  The function computes this by generating Fibonacci numbers modulo n
 *  until the Pisano period starts again (state 0,1).  During the run it
 *  records the first time each residue occurs and returns one plus the
 *  largest such first occurrence.  This is exact for any seq_t that is
 *  an unsigned integer type; the algorithm runs in O(period(n)) time
 *  and O(n) memory, which is well under the limits for n ≤ 10000.  */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case n = 1: Fibonacci mod 1 is always 0, so the set is {0}
     * and the first occurrence is at i = 0, giving k = 1.  */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Allocate an array that stores, for each residue r (0 ≤ r < n),
     * the index of its first occurrence in the Fibonacci sequence
     * modulo n.  We initially mark all entries as UNSET.  */
    size_t N = (size_t)n;
    idx_t *first = (idx_t *)malloc(N * sizeof(idx_t));
    if (!first)
        return (seq_t)0;   /* OOM; return 0 as error marker (unlikely in tests). */

    const idx_t UNSET = (idx_t)-1;
    for (size_t i = 0; i < N; ++i)
        first[i] = UNSET;

    /* Fibonacci residues: F(0) = 0, F(1) = 1 (mod n).  */
    first[0] = 0;                     /* F(0) occurs at i = 0          */
    idx_t curr = 1 % n;                /* F(1) mod n                    */
    if (curr != 0)                     /* In the usual case n > 1       */
        first[curr] = 1;               /* first[1] = 1                  */

    idx_t prev = 0;                    /* F(0)                          */
    idx_t i = 2;                       /* Current index                    */

    for (;; ++i) {
        idx_t next = (prev + curr) % n;
        prev = curr;
        curr = next;

        if (first[curr] == UNSET)
            first[curr] = i;          /* Record first appearance     */

        if (prev == (idx_t)0 && curr == (idx_t)1)
            break;                    /* Pisano period has restarted */
    }

    /* Find the largest first-occurrence index among residues that
     * actually appeared (first != UNSET).  The answer is that index + 1. */
    idx_t maxpos = 0;
    for (size_t r = 0; r < N; ++r) {
        if (first[r] != UNSET && first[r] > maxpos)
            maxpos = first[r];
    }

    free(first);
    return (seq_t)(maxpos + (idx_t)1);
}
