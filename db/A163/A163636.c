/*
 * A163636  The sum of all odd numbers from 2n-1 up to the n-th odd nonprime.
 * https://oeis.org/A163636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A014076(n)+2*n-1)*(A014076(n)-2*n+3)/4
 * Range:     u8 n <= 7, u16 n <= 211, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   nth_odd_nonprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): sum of all odd numbers from 2n-1 up to the n‑th odd nonprime. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/nth_odd_nonprime.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n = 0; return 0 safely */
        return (seq_t)0;

    seq_t odd_n = nth_odd_nonprime(n);

    seq_t term = ((odd_n + ((seq_t)2 * (seq_t)n - (seq_t)1)) *
                  (odd_n - ((seq_t)2 * (seq_t)n) + (seq_t)3)) / (seq_t)4;
    return term;
}
