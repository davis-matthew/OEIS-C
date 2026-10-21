/*
 * A092919  Partial sums of A000193 (round(log(n))).
 * https://oeis.org/A092919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A092919: Partial sums of A000193 (round(log(n))).
   Formula: a(n) = Σ_{k=1..n} round(log(k)),
   with offset 1 (so a(1) = round(log(1)) = 0). */

#include <math.h>
#include <stdint.h>
#include <stddef.h>

/* External linkage function as required. */
seq_t a(idx_t n)
{
    if (n == 0)      /* safety for indices below the offset */
        return 0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute the integer round of the natural logarithm of k. */
        double d = log((double)k);
        seq_t term = (seq_t)llround(d);    /* round to nearest integer */
        sum += term;
    }
    return sum;
}
