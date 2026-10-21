/*
 * A205444  Least s(k) such that n divides s(k)-s(j) for some j<k, where
 *          s(j)=Fibonacci(2j-1).
 * https://oeis.org/A205444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the first odd‑indexed Fibonacci number s(k) for which the
 *            residue mod n repeats. Computed by generating odd Fibonacci
 *            values until a residue has already been seen modulo n, then
 *            returning that Fibonacci value
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 24, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* not defined for n=0, return 0 */

    /* Allocate a table of residues modulo n. */
    size_t cap = (size_t)n;
    bool *seen = (bool *)calloc(cap, sizeof(bool));
    if (!seen)
        return (seq_t)0; /* allocation failure */

    /* Fibonacci seed values: F0 = 0, F1 = 1. */
    seq_t f_prev = 0; /* F0 */
    seq_t f_curr = 1; /* F1 */

    /* i is the Fibonacci index (starting at 1 for F1). */
    idx_t i = 1;

    /* Process s(1) = F1. */
    idx_t r = (idx_t)(f_curr % (seq_t)n);
    seen[(size_t)r] = true;

    for (;;)
    {
        /* Advance Fibonacci sequence: next = prev + curr. */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
        i++; /* next Fibonacci index */

        /* Only odd Fibonacci indices correspond to s(k). */
        if (i & 1U)
        {
            idx_t rem = (idx_t)(f_curr % (seq_t)n);
            if (seen[(size_t)rem])
            {
                free(seen);
                return f_curr; /* first repeated residue found */
            }
            seen[(size_t)rem] = true;
        }
    }
}
