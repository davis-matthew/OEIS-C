/*
 * A014633  Even pentagonal numbers.
 * https://oeis.org/A014633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; for n ≥ 1, a(n) =
 *            ((1-3*(-1)^(n)+12*n)(1-(-1)^(n)+4*n)) / 8
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (This is the OEIS formula given for offset 0.  The first factor
    uses (n+1) which becomes n when we shift the index by one,
    so we compute the formula with n directly.) */
#include <stdint.h>   /* for uint64_t used in idx_t */

seq_t a(idx_t n)
{
    if (n == 0)               /* a(0) = 0 */
        return (seq_t)0;

    /* use n in the formula (since n>=1) */
    idx_t np1 = n;                            /* np1 = n+1 in the OEIS formula, but n here */
    /* sign = (-1)^n :  1 if np1 even, -1 if np1 odd */
    long long sign = (np1 & 1LL) ? -1LL : 1LL;

    long long t1 = 1 - 3*sign + 12*(long long)np1;   /* first factor */
    long long t2 = 1 - sign  + 4*(long long)np1;    /* second factor */
    long long prod = t1 * t2;                       /* fits in 64‑bit signed */

    return (seq_t)(prod / 8LL);                     /* division is exact integer */
}
