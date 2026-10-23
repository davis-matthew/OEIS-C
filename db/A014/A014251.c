/*
 * A014251  a(n) = b(n) - c(n) where b(n) is the n-th Fibonacci number greater
 *          than 2 and c(n) is the n-th number not in sequence b( ).
 * https://oeis.org/A014251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014251
 * a(n) = b(n) - c(n)
 *   b(n) = n-th Fibonacci number > 2  = F(n+3)
 *   c(n) = n-th positive integer NOT equal to any Fibonacci number > 2
 *
 * The implementation finds c(n) by iterating over consecutive Fibonacci
 * numbers and counting the gaps between them.  Because Fibonacci numbers
 * are very sparse, only a few dozen iterations are needed even for
 * large n, keeping the call time well below the 2‑second limit.
 */

#include <stdint.h>
#include "../utils/fib.h"

/* Find the n-th positive integer that is not a Fibonacci number > 2 */
static seq_t find_c(idx_t n)
{
    seq_t prev = 0;   /* previous Fibonacci number counted so far (0 handles 1,2) */
    idx_t idx = 4;    /* F(4) = 3 is the first Fibonacci > 2 */
    seq_t missing = 0;/* numbers missing up to the current gap */

    for (;;) {
        seq_t curr = fib(idx);              /* next Fibonacci number */
        seq_t gap = curr - prev - 1;        /* integers between prev and curr */
        if (gap > 0) {
            if (missing + gap >= n) {
                /* Desired missing number lies inside this gap. */
                return prev + (n - missing);
            }
            missing += gap;                 /* accumulate missing numbers */
        }
        prev = curr;                       /* move to next interval */
        idx++;                             
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n-th Fibonacci number greater than 2 */
    seq_t fn = fib(n + 3);

    /* n-th number not in the Fibonacci sequence > 2 */
    seq_t cn = find_c(n);

    return fn - cn;
}
