/*
 * A121696  Number of even-length first columns in all deco polyominoes of
 *          height n. A deco polyomino is a directed column-convex polyomino
 *          in which the height, measured along the diagonal, is attained only
 *          in the last column.
 * https://oeis.org/A121696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + n*(n-2)!*floor((n-1)/2), a(1) = 0, a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* The types seq_t and idx_t are provided by the build system.  
   Their definitions can be uint8_t, uint16_t, uint32_t, uint64_t,
   unsigned __int128, or an arbitrary-precision integer class.  */

/* Return the n-th term of OEIS A121696.  */
seq_t a(idx_t n)
{
    /* Base cases.  Values below the offset are not defined; return 0. */
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* We compute the sequence iteratively, keeping only the latest
       value of each parity (even and odd).  */
    seq_t last_even = (seq_t)1;     /* a(2) */
    seq_t last_odd  = (seq_t)0;     /* a(1) */
    seq_t current   = last_odd;     /* placeholder for the final result */

    /* factorial of fact_k = fact_k!  (fact_k is the current exponent).  */
    seq_t fact = (seq_t)1;          /* 0! = 1 */
    idx_t fact_k = 0;               /* current exponent of fact */

    /* Iterate from 3 up to n, updating factorial and sequence values. */
    for (idx_t i = 3; i <= n; ++i) {
        /* Ensure fact == (i-2)! before using it. */
        while (fact_k < (i - 2)) {
            fact_k++;
            fact = fact * (seq_t)fact_k;
        }

        /* term = n * floor((n-1)/2) * (n-2)!  */
        seq_t term = ((seq_t)i) * ((seq_t)((i - 1) / 2));
        term = term * fact;

        /* a(i) = a(i-2) + term.  a(i-2) has the same parity as i. */
        seq_t prev = (i & 1) ? last_odd : last_even;
        current = prev + term;

        /* Update the last value for the current parity. */
        if ((i & 1) == 0)
            last_even = current;
        else
            last_odd  = current;
    }

    return current;
}
