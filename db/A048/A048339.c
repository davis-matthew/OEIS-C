/*
 * A048339  a(n) in base 15 is a repdigit.
 * https://oeis.org/A048339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n - 14*floor((n-1)/14))*(15^floor((n+13)/14) - 1)/14
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 113, u64 n <= 226,
 *            u128 n <= 452, big any n
 * Helpers:   big_pow15, power15_128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow15.h"
#include "../utils/power15_128.h"

#ifdef __cplusplus

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t k1 = (n - 1) / 14;
    idx_t exp = (n + 13) / 14;
    seq_t digit = seq_t(n - 14 * k1);

    seq_t pow15 = big_pow15(exp);
    seq_t diff   = pow15 - 1;
    seq_t q      = diff / 14;

    return digit * q;
}
#else

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t k1 = (n - 1) / 14;
    idx_t exp = (n + 13) / 14;
    seq_t digit = (seq_t)(n - 14 * k1);

    unsigned __int128 pow15 = 1;
    for (idx_t i = 0; i < exp; ++i)
    {
        pow15 *= 15;
    }
    unsigned __int128 diff = pow15 - 1;
    unsigned __int128 q    = diff / 14;
    unsigned __int128 res  = (unsigned __int128)digit * q;

    return (seq_t)res;
}
#endif
