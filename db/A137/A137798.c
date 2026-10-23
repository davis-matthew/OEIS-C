/*
 * A137798  Partial sums of A137797.
 * https://oeis.org/A137798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0}^{n-1} ( 2*((k+1) mod 5) - 2*((k+1) mod 2) ),
 *            with a(0)=0. Periodic summand length 10, sum per cycle 30
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Prefix sums of one period (10 terms) of the sequence
       0, 4, 4, 8, –2, 2, 2, 6, 6, 0. */
    static const seq_t prefix[10] = {
        (seq_t)0,  /* sum of first 0 terms  */
        (seq_t)0,  /* sum of first 1 term   */
        (seq_t)4,  /* sum of first 2 terms  */
        (seq_t)8,  /* sum of first 3 terms  */
        (seq_t)16, /* sum of first 4 terms  */
        (seq_t)14, /* sum of first 5 terms  */
        (seq_t)16, /* sum of first 6 terms  */
        (seq_t)18, /* sum of first 7 terms  */
        (seq_t)24, /* sum of first 8 terms  */
        (seq_t)30  /* sum of first 9 terms  */
    };

    seq_t m = (seq_t)n;              /* number of terms to sum */
    seq_t cycles = m / (seq_t)10;    /* complete 10‑term cycles   */
    size_t rem = (size_t)(m % (seq_t)10); /* remaining terms */

    return cycles * (seq_t)30 + prefix[rem];
}
