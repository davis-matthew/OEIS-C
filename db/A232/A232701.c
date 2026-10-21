/*
 * A232701  a(n) = (2*n-1)!! mod n!, where double factorial is A006882.
 * https://oeis.org/A232701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n-1)!! mod n!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 0 because 1!! ≡ 1 (mod 1) */
    if (n == 0) return 0;

    /* Compute n! and (2n‑1)!! using 128‑bit intermediates
       which are sufficient for all n where a(n) fits in seq_t. */
    unsigned __int128 fact = 1;
    unsigned __int128 prod = 1;

    for (idx_t i = 2; i <= n; ++i)
        fact *= (unsigned __int128)i;

    for (idx_t i = 1; i <= n; ++i)
        prod *= ((unsigned __int128)2 * i - 1);

    unsigned __int128 res = prod % fact;
    return (seq_t)res;
}
