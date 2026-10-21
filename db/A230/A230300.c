/*
 * A230300  a(n) = n + wt(n-1), where wt() = A000120() is the binary weight.
 * https://oeis.org/A230300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + wt(n-1) (wt = binary weight)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute a(n) for any n >= 1.
   Uses a simple loop to count the set bits of (n-1). */
seq_t a(idx_t n)
{
    /* Edge case (should not occur for the official use) */
    if (n == 0)
        return (seq_t)0;

    idx_t m = n - 1;      /* value to examine */
    unsigned int w = 0;   /* bit count */

    /* Count set bits using the Kernighan algorithm */
    while (m != 0) {
        m &= (m - 1);
        ++w;
    }

    /* Convert to seq_t and return the result */
    return (seq_t)n + (seq_t)w;
}
