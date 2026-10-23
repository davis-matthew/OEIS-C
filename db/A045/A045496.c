/*
 * A045496  Packing density for n balls in complex projective plane using
 *          Fubini-Study metric (numerators).
 * https://oeis.org/A045496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = a(n-1) + c(n) for n ≥ 1, where
 * Range:     u8 n <= 7, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* c(1)=1, c(2)=0, c(3)=2, c(4)=-2, c(5)=19, c(6)=4, c(7)=39, */
/* c(8)=225, c(9)=-287 and c(n)=0 for n ≥ 10.  Thus a(n)=1 for n ≥ 10. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* coefficients c(n) for 1 ≤ n ≤ 9 (index 0 unused) */
    static const int coeff[10] = {0, 1, 0, 2, -2, 19, 4, 39, 225, -287};

    if (n >= 10)
        return (seq_t)1;        /* values stabilize at 1 from n = 10 onward */

    unsigned long long sum = 0;
    for (idx_t i = 1; i <= n; ++i)
        sum += coeff[(size_t)i];
    return (seq_t)sum;
}
