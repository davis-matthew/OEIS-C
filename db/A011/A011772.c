/*
 * A011772  Smallest number m such that m(m+1)/2 is divisible by n.
 * https://oeis.org/A011772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look, nice
 * Formula:   a(n) = the smallest m ≥ 1 such that m(m+1)/2 is divisible by n
 * Range:     u8 n <= 255, u16 n <= 9762, u32 n <= 9762, u64 n <= 9762,
 *            u128 n <= 9762, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n ≥ 1 the upper bound a(n) ≤ 2n‑1 follows from n | m(m+1)/2 ⇒
       m(m+1) ≡ 0 (mod 2n) ⇒ m ≡ 0 or –1 (mod 2n).  Hence we only need
       to search m < 2n. */
    if (n == 0)        /* the sequence is defined for n≥1; return 0 otherwise */
        return 0;

    const idx_t limit = n * 2u;          /* < 2n */
    for (idx_t m = 1; m < limit; ++m)
    {
        /* Compute T(m) = m(m+1)/2 using 128‑bit arithmetic to avoid overflow
           and then test divisibility by n.  The 128‑bit type is available on
           GCC/Clang; it is a common extension for C11/C++17. */
        __int128 prod = (__int128)m * (m + 1) / 2;
        if ((prod % n) == 0)
            return (seq_t)m;
    }
    /* The loop always terminates because at worst m = 2n-1 gives T(m) divisible
       by n (when n is a power of two); this line is only a safety guard. */
    return (seq_t)(limit - 1);
}
