/*
 * A205781  Least positive integer j such that n divides C(k)-C(j), where k,
 *          as in A205780, is the least number for which there is such a j,
 *          and C=A007598 (squared Fibonacci numbers).
 * https://oeis.org/A205781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A205781:  least positive j such that
   n divides C(k)-C(j), where C(k)=F(k)^2 is the squared Fibonacci sequence.
   This finds the first repeated residue of C(k) modulo n; the earlier
   occurrence of that residue is the desired j. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate array to remember the first occurrence of each residue
       of C(k) modulo n (0 .. n-1). */
    idx_t *first = (idx_t*)calloc((size_t)n, sizeof(idx_t));
    if (first == NULL)
        return (seq_t)0;   /* out of memory – caller can ignore */

    /* Fibonacci numbers modulo n: F(1)=1, F(2)=1. */
    idx_t fprev = 1 % n;   /* F(1) */
    idx_t fn    = 1 % n;   /* F(2) */

    for (idx_t k = 1; ; ++k) {
        /* Residue of C(k) = F(k)^2 mod n. */
        unsigned __int128 prod = (unsigned __int128)fn * fn;
        idx_t residue = (idx_t)(prod % (unsigned __int128)n);

        /* If this residue has been seen before, we have found the
           minimal repeat; return its first occurrence j. */
        if (first[residue] == 0) {
            first[residue] = k;          /* record first occurrence */
        } else {
            seq_t j = (seq_t)first[residue];
            free(first);
            return j;
        }

        /* Advance Fibonacci numbers for next k. */
        idx_t fnext = (fprev + fn) % n;
        fprev = fn;
        fn    = fnext;
    }
}
