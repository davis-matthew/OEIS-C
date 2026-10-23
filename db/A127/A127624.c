/*
 * A127624  An 11th-order Fibonacci sequence: a(n) = a(n-1) + ... + a(n-11).
 * https://oeis.org/A127624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 40, u64 n <= 72, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A127624
   Formula: a(n) = a(n-1) + a(n-2) + ... + a(n-11)
   Initial values: a(1)=a(2)=...=a(11)=1
*/
#include <stddef.h>     /* for size_t (not used but harmless) */

seq_t a(idx_t n)
{
    /* handle the trivial initial segment */
    if (n <= 11)
        return (seq_t)1;

    /* ring buffer holding the last 11 values */
    seq_t ring[11];
    for (idx_t i = 0; i < 11; ++i)
        ring[i] = (seq_t)1;

    /* current sum of the last 11 values */
    seq_t sum  = (seq_t)11;
    seq_t last = (seq_t)0;   /* will hold a(n) after the loop */

    /* compute a(12) .. a(n) */
    for (idx_t i = 12; i <= n; ++i) {
        seq_t new_val = sum;                     /* a(i) = sum of last 11 */
        idx_t pos    = (i - 12) % 11;            /* position in the ring to replace */
        sum   = sum + new_val - ring[pos];       /* update the sum for next step */
        ring[pos] = new_val;                     /* store the new value */
        last = new_val;                          /* keep the latest value */
    }
    return last;
}
