/*
 * A096557  Consecutive internal states of the linear congruential
 *          pseudo-random number generator rand() used in Microsoft's Visual
 *          C++.
 * https://oeis.org/A096557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑1) * 214013 + 2531011 (mod 2^32), a(1) = 0
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of the Microsoft Visual C++ PRNG sequence A096557.
   The recurrence is applied starting from a(1)=0, truncating each
   intermediate result to 32 bits.
*/
seq_t a(idx_t n)
{
    seq_t val = 0;               /* a(1) = 0 */
    if (n < 2) return val;       /* for n == 1 */

    /* Iterate from a(2) up to a(n). */
    for (idx_t i = 2; i <= n; ++i) {
        /* Perform the multiplication/floor addition in 64‑bit to avoid
           overflow of the 32‑bit accumulator.  The final result is
           masked to the lower 32 bits, matching the definition. */
        uint64_t temp = (uint64_t)(unsigned long long)val * 214013ULL + 2531011ULL;
        val = (seq_t)(temp & 0xFFFFFFFFULL);
    }
    return val;
}
