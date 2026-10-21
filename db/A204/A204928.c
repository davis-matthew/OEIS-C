/*
 * A204928  s(k(n)) - s(j(n)), where (s(k(n)), s(j(n))) is the least pair of
 *          distinct Fibonacci numbers for which n divides s(k(n)) - s(j(n)).
 * https://oeis.org/A204928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 58, u32 n <= 192, u64 n <= 590,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence A204928 is defined as follows:

   a(n) = minimal positive difference d such that
   there exist distinct Fibonacci numbers s(k), s(j)
   with s(k)-s(j)=d and n | d.

   Equivalently, a(n) is the smallest positive difference
   between two distinct Fibonacci numbers that are congruent
   modulo n.  The difference is obtained by finding the first
   pair of equal residues modulo n in the Fibonacci sequence
   (skipping equal values) and taking the difference of the
   corresponding Fibonacci numbers. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Note: idx_t (uint64_t) and seq_t are supplied by the build.
 * We use only + - * / % and comparisons on seq_t.               */

seq_t a(idx_t n)
{
    /* handle trivial case n==0 is undefined – callers will never pass it */
    if (n == 0) return 0;

    /* Upper bound on Pisano period is at most 6*n. We add a small
       safety margin so we don't overflow arrays.                     */
    size_t limit = 6 * n + 2;

    /* Allocate Fibonacci array.  Use malloc (works for arbitrary
       integer types).                                              */
    seq_t *fib = (seq_t *)malloc(limit * sizeof(seq_t));
    if (!fib) return 0;            /* allocation failure */

    /* Track first occurrence of each residue modulo n.
     * Use -1 for “not seen yet”.                                   */
    int *first = (int *)malloc(n * sizeof(int));
    if (!first) { free(fib); return 0; }
    memset(first, 0xff, n * sizeof(int));  /* set to -1 */

    /* Fibonacci indices start at 0 */
    size_t i = 0;
    fib[0] = 0;
    int mod_prevprev = 0;                 /* F_0 mod n */
    first[mod_prevprev] = 0;              /* F_0 appears first at index 0 */

    /* Handle index 1 */
    i = 1;
    fib[1] = 1;
    int mod_prev = (int)(1 % n);         /* F_1 mod n */
    if (first[mod_prev] == -1) {
        first[mod_prev] = 1;
    } else {
        /* difference would be zero only if the two Fibonacci
           numbers are equal (this occurs only for n=1 with
           F_1=F_2=1).  In that case skip. */
        if (fib[1] != fib[first[mod_prev]]) {
            seq_t res = fib[1] - fib[first[mod_prev]];
            free(first); free(fib);
            return res;
        }
    }

    /* Main loop: generate Fibonacci numbers, check residues,
       until a suitable pair is found.                         */
    for (i = 2; i < limit; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];

        int mod_i = (mod_prev + mod_prevprev) % (int)n;

        if (first[mod_i] == -1) {
            first[mod_i] = (int)i;
        } else {
            /* Skip the case where the Fibonacci numbers are equal
               (happens only for the equal pair 1,1).           */
            if (fib[i] != fib[first[mod_i]]) {
                seq_t res = fib[i] - fib[first[mod_i]];
                free(first); free(fib);
                return res;
            }
        }

        mod_prevprev = mod_prev;
        mod_prev = mod_i;
    }

    /* Should never reach here for n >= 1: a return idempotent fallback. */
    free(first); free(fib);
    return 0;
}
