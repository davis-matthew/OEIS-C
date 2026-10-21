/*
 * A095386  Largest prime factor of peak values of 3x+1 trajectory started at
 *          n.
 * https://oeis.org/A095386
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of the peak of the Collatz (3x+1)
 *            trajectory started at n
 * Range:     u8 n <= 26, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n < 2u)   /* sequence is defined only for n ≥ 2 */
        return 0;

    seq_t curr = (seq_t)n;
    seq_t peak = curr;

    /* traverse Collatz trajectory until reaching 1, track maximum value */
    while (curr > 1) {
        if ((curr & 1) == 0)       /* even */
            curr >>= 1;
        else                       /* odd */
            curr = curr * 3 + 1;

        if (curr > peak)
            peak = curr;
    }

    /* peak fits in 64‑bit for n ≤ 10000 (the test range). */
    uint64_t peak64 = (uint64_t)peak;
    uint64_t lpf64  = largest_prime_factor(peak64);
    return (seq_t)lpf64;
}
