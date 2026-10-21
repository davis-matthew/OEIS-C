/*
 * A221158  Fibonacci numbers with two 1's in the binary representation.
 * https://oeis.org/A221158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   has_two_one_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A221158: Fibonacci numbers of the form 2^a + 2^b (a>b). */
/* The function finds the nth term by iterating Fibonacci numbers and selecting
   those with exactly two set bits in their binary representation. The
   known terms are 3, 5, 34, and 144. */

#include <stdbool.h>
#include "../utils/has_two_one_bits.h"

seq_t a(idx_t n)
{
    /* Static table and initialization flag to store the discovered terms
       (there are only four of them). */
    static seq_t terms[4];
    static idx_t count = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Generate Fibonacci numbers until all known terms have been found. */
        seq_t f0 = (seq_t)0;   /* F0 */
        seq_t f1 = (seq_t)1;   /* F1 */
        while (count < 4) {
            seq_t f2 = f0 + f1;   /* F(n+1) = F(n) + F(n-1) */
            f0 = f1;
            f1 = f2;
            if (has_two_one_bits(f2))
                terms[count++] = f2;
        }
        initialized = true;
    }

    /* Return the requested term if it exists; otherwise return 0. */
    return (n <= count) ? terms[n - 1] : ((seq_t)0);
}
