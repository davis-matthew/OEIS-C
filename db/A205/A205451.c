/*
 * A205451  The index j<k such that n divides s(k)-s(j) for some j, where
 *          s(j)=Fibonacci(2j), and k is the least index for which such a j
 *          exists.
 * https://oeis.org/A205451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the earlier index j such that n divides s(k)-s(j), where
 *            * s(j)=Fibonacci(2j) and k is the smallest index for which such
 *            a j exists. * Implementation: scan the Fibonacci sequence modulo
 *            n, keep only even * indices (s(i)), and return the first earlier
 *            residue when a repetition * occurs
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the nth term of OEIS A205451. */
seq_t a(idx_t n)
{
    if (n == 0)                  /* defined only for n ≥ 1 */
        return 0;

    /* first[residue] stores the first j (index) where that residue appeared.
       The residue range is 0 .. n-1. */
    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!first)
        return 0;              /* allocation failure */

    /* Fibonacci variables: f_prev = F(k-1), f_curr = F(k) modulo n. */
    idx_t f_prev = 0;           /* F(0) % n */
    idx_t f_curr = 1 % n;       /* F(1) % n */
    idx_t k = 1;                /* current Fibonacci index */

    for (;;)
    {
        /* Advance to next Fibonacci number: F(k+1). */
        idx_t f_next = f_prev + f_curr;
        if (f_next >= n)
            f_next -= n;       /* modulo n */

        f_prev = f_curr;
        f_curr = f_next;
        ++k;

        /* If k is even, we have the term s(i) = F(k) with i = k/2. */
        if ((k & 1) == 0) {
            idx_t i = k >> 1;                  /* i = k/2 */
            idx_t residue = f_curr;            /* residue modulo n */

            if (first[residue] != 0) {          /* repeated residue found */
                seq_t result = (seq_t)first[residue];
                free(first);
                return result;
            }
            first[residue] = i;                 /* record first occurrence */
        }

        /* For safety, break if we have examined more than 2*n+2 terms.
           In practice a repetition will have been found long before. */
        if (k > ((idx_t)2 * n) + 2) {
            free(first);
            return 0;                          /* should never happen */
        }
    }
}
