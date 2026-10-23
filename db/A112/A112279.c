/*
 * A112279  a(1)=1; a(n)=prime(mod(a(n-1),100)).
 * https://oeis.org/A112279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less, changed
 * Formula:   a(1)=1; a(n)=prime(mod(a(n-1),100))
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* This sequence becomes periodic with period 5 after a(3).
       Compute the first 5 periodic values once and reuse them
       for any n.  The periodic block is {5,11,31,127,103}. */
    static seq_t period[5];
    static bool initialized = false;

    if (!initialized) {
        seq_t prev = (seq_t)1;          /* a(1) */
        seq_t curr = (seq_t)0;          /* will hold a(i) */
        /* Compute a(2) through a(8) to extract the period. */
        for (idx_t i = 2; i <= 8; ++i) {
            seq_t idx = prev % (seq_t)100;
            curr  = get_prime((uint64_t)idx);
            if (i >= 4) period[i - 4] = curr;   /* store a(4) .. a(8) */
            prev = curr;
        }
        initialized = true;
    }

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    idx_t pos = (n - 4) % 5;       /* index into the period */
    return period[pos];
}
