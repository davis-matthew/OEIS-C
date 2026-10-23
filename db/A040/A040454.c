/*
 * A040454  Continued fraction for sqrt(476).
 * https://oeis.org/A040454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(0) = 21;   for n ≥ 1, the continued‑fraction terms of
   √476 repeat with period [1,4,2,10,2,4,1,42].
   Thus a(n) = period[(n‑1) mod 8] for n ≥ 1.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* fixed periodic part of the continued fraction of √476 */
    static const seq_t period[8] = {
        (seq_t)1, (seq_t)4, (seq_t)2, (seq_t)10,
        (seq_t)2, (seq_t)4, (seq_t)1, (seq_t)42
    };

    if (n == 0)
        return (seq_t)21;

    /* (n-1) is safe because n>0 here */
    idx_t idx = (n - 1) % 8;
    return period[idx];
}
