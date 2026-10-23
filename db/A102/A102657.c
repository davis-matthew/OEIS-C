/*
 * A102657  Numbers k such that 11*k^2 + 11*k + 1 is prime.
 * https://oeis.org/A102657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 32, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102657: numbers k such that 11*k^2 + 11*k + 1 is prime.
   The function enumerates k from 1 upwards, testing the quadratic
   expression for primality until the nth such k is found.  */

/* The build supplies the following types; no definitions are needed. */
#include <stdbool.h>
#include "../utils/is_prime_seq.h"  /* provides: bool is_prime_seq(seq_t); */

seq_t a(idx_t n)
{
    if (n < 1)                /* offset is 1; for n==0 return 0 (undefined) */
        return (seq_t)0;

    idx_t count = 0;          /* number of qualifying k found */
    idx_t k = 0;              /* current k being tested */

    while (count < n) {
        k++;
        seq_t kk = (seq_t)k;
        /* compute 11*k^2 + 11*k + 1 = 11*k*(k+1) + 1 */
        seq_t val = ((seq_t)11 * kk * (kk + 1)) + 1;
        if (is_prime_seq(val))
            count++;
    }

    return (seq_t)k;
}
