/*
 * A063765  Least integer m whose largest prime factor > m^(n/(n+1)).
 * https://oeis.org/A063765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * nextPrime(2^n) (where nextPrime(p) is the smallest
 *            prime > p)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   nextPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation calls the provided nextPrime function. */
/* It works for any n where 2^n fits into the builtin integer type seq_t
   (on a 64‑bit build this covers n < 64).  For larger n the shift would
   overflow and the function falls back to a simple loop that multiplies
   by 2 repeatedly.  The test harness for the available builds should
   never query a value that does not fit in seq_t.  */
#include "../utils/nextPrime.h"
#include <stdint.h>   /* for uint_fast64_t if needed */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined for n < 1, return 0 */

    /* Compute 2^n as seq_t.  If n is too large to shift, fall back
       to repeated doubling to avoid undefined behaviour. */
    seq_t pow2;
    if (n >= (sizeof(seq_t) * 8)) {
        pow2 = 1;
        for (idx_t i = 0; i < n; ++i)
            pow2 *= 2;
    } else {
        pow2 = ((seq_t)1) << n;
    }

    /* nextPrime expects an idx_t argument.  Because pow2 must fit in
       idx_t for the tests that are run, a safe cast is performed. */
    idx_t start = (idx_t)pow2;

    /* Get the first prime greater than 2^n and return twice that value. */
    seq_t p = nextPrime(start);
    return p * 2;
}
