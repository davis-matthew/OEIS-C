/*
 * A126328  Rounded value of n!/(n(n+1)/2); A000142(n)/A000217(n).
 * https://oeis.org/A126328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n!/(n(n+1)/2)) = round(2*(n-1)!/(n+1))
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the domain defined for n >= 1; n == 0 is undefined but guard */
    if (n == 0) return (seq_t)0;

    /* Denominator for the rounded formula: d = n+1  */
    idx_t d = n + 1;
    /* unsigned __int128 allows us to compute the factorial for all n
       that produce results fitting into seq_t without overflow.           */
    unsigned __int128 num = 1;

    /* Compute (n-1)!  */
    for (idx_t i = 2; i <= n - 1; ++i)
        num *= i;

    /* Multiply by 2 (the factor from the original formula) */
    num <<= 1;

    /* Addition for rounding: floor((num + d/2) / d) */
    num += (unsigned __int128)(d / 2);

    /* Compute the rounded division */
    unsigned __int128 res = num / d;

    /* Cast back to seq_t (truncates if the value does not fit) */
    return (seq_t)res;
}
