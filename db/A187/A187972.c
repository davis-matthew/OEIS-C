/*
 * A187972  a(n) = [nr+kr]-[nr]-[kr], where r=sqrt(2), k=4, [ ]=floor.
 * https://oeis.org/A187972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [ (n+4)*sqrt(2) ] - [ n*sqrt(2) ] - 5 (with sqrt2 = √2
 *            and [ ] = floor)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _isqrt_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_isqrt_uint128.h"

/* Sequence entry */
seq_t a(idx_t n)
{
    /* offset 1: a(1) is first term */
    if (n == 0)
        return 0;  /* undefined, but return 0 to be safe */

    idx_t n4 = n + 4;

    /* compute floor(n*sqrt(2)) = floor(sqrt(2*n^2)) */
    unsigned __int128 n2 = (unsigned __int128) n * n;
    seq_t t1 = _isqrt_uint128((unsigned __int128)2 * n2);

    /* compute floor((n+4)*sqrt(2)) = floor(sqrt(2*(n+4)^2)) */
    unsigned __int128 n4_2 = (unsigned __int128) n4 * n4;
    seq_t t2 = _isqrt_uint128((unsigned __int128)2 * n4_2);

    /* a(n) = t2 - t1 - 5, which is 0 or 1 */
    return (seq_t)(t2 - t1 - 5);
}
