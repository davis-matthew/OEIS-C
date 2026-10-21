/*
 * A066853  Number of different remainders (or residues) for the Fibonacci
 *          numbers (A000045) when divided by n (i.e., the size of the set of
 *          F(i) mod n over all i).
 * https://oeis.org/A066853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct residues in the Fibonacci sequence
 *            modulo n
 * Range:     u8 n <= 298, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Guard against an invalid offset; the sequence is defined for n>=1. */
    if (n == 0)
        return (seq_t)0;

    /* Allocate a table of n booleans to record which residues have
       already been seen.  Each boolean is one byte, sufficient for
       n up to 10 000 and portable to all builds. */
    bool *seen = (bool *)malloc((size_t)n * sizeof(bool));
    if (!seen)
        return (seq_t)0;                 /* allocation failure → 0 */

    for (size_t i = 0; i < (size_t)n; ++i)
        seen[i] = false;

    seq_t mod   = (seq_t)n;               /* current modulus */
    seq_t x     = (seq_t)0;               /* F(k)   */
    seq_t y     = (seq_t)1 % mod;         /* F(k+1) */

    seq_t count = (seq_t)0;
    /* Count the first residue (0). */
    seen[(size_t)x] = true;
    ++count;

    /* Generate Fibonacci residues until the Pisano period
       (0, 1) reappears after at least one iteration. */
    while (true)
    {
        seq_t next = (x + y) % mod;      /* F(k+2) mod n */

        x = y;                           /* shift */
        y = next;

        if (!seen[(size_t)x])
        {
            seen[(size_t)x] = true;
            ++count;
        }

        if (x == (seq_t)0 && y == (seq_t)(1 % mod))
            break;
    }

    free(seen);
    return count;
}
