/*
 * A071408  a(n+1) - 2*a(n) + a(n-1) = (2/3)*(1 + w^(n+1) + w^(2*n+2)) with
 *          a(1)=0, a(2)=1, and where w is the imaginary cubic root of unity.
 * https://oeis.org/A071408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*(n-1)/3) - 1 (OEIS A071408)
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for n >= 1; offset 1 (a(1)=0)  */
seq_t a(idx_t n)
{
    /* For safety return 0 for n==0 (index below the offset) */
    if (n == 0)
        return 0;

    seq_t N = (seq_t)n;

    /* Compute floor(n*(n-1)/3) using 128‑bit arithmetic to avoid
       overflow even when N is large. */
    unsigned __int128 prod = (unsigned __int128)n * (n - 1);
    seq_t floor_term = (seq_t)(prod / 3);

    /* Final result: a(n) = n + floor(n*(n-1)/3) - 1 */
    return N + floor_term - 1;
}
