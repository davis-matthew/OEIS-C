/*
 * A210432  Number of semistandard Young tableaux over all partitions of 10
 *          with maximal element <= n.
 * https://oeis.org/A210432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 16, u64 n <= 153, u128 n <= 351,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A210432: Number of semistandard Young tableaux over all partitions of 10 with maximal element <= n.
   Formula: a(n) = n^2*(126432+206020*n^2+101031*n^4+18930*n^6+1187*n^8) / 453600
*/
#ifdef __cplusplus
#include <type_traits>

seq_t a(idx_t n)
{
    /* Path for integral seq_t types up to 64 bits: use 128‑bit arithmetic to avoid overflow. */
    if constexpr (std::is_integral<seq_t>::value && sizeof(seq_t) <= 8)
    {
        unsigned __int128 N  = n;
        unsigned __int128 t  = N * N;                       /* n^2                  */
        unsigned __int128 sum = (unsigned __int128)126432;
        sum += (unsigned __int128)206020 * t;                /* 206020*n^2           */
        sum += (unsigned __int128)101031 * t * t;           /* 101031*n^4           */
        sum += (unsigned __int128)18930  * t * t * t;        /* 18930*n^6            */
        sum += (unsigned __int128)1187   * t * t * t * t;    /* 1187*n^8             */
        unsigned __int128 num  = t * sum;                   /* n^2 * sum            */
        unsigned __int128 res128 = num / (unsigned __int128)453600;
        /* res128 < 2^64 for all n whose result fits in 64 bits */
        return (seq_t)(unsigned long long)res128;
    }
    else
    {
        /* Path for arbitrary‑precision seq_t: perform the calculation with the big‑int type itself. */
        seq_t N  = (seq_t)n;
        seq_t t  = N * N;                       /* n^2                  */
        seq_t sum = (seq_t)126432;
        sum += (seq_t)206020 * t;                /* 206020*n^2           */
        sum += (seq_t)101031 * t * t;           /* 101031*n^4           */
        sum += (seq_t)18930  * t * t * t;        /* 18930*n^6            */
        sum += (seq_t)1187   * t * t * t * t;    /* 1187*n^8             */
        seq_t num = t * sum;                   /* n^2 * sum            */
        seq_t res = num / (seq_t)453600;
        return res;
    }
}
#else
#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned __int128 N  = n;
    unsigned __int128 t  = N * N;                       /* n^2                  */
    unsigned __int128 sum = (unsigned __int128)126432;
    sum += (unsigned __int128)206020 * t;                /* 206020*n^2           */
    sum += (unsigned __int128)101031 * t * t;           /* 101031*n^4           */
    sum += (unsigned __int128)18930  * t * t * t;        /* 18930*n^6            */
    sum += (unsigned __int128)1187   * t * t * t * t;    /* 1187*n^8             */
    unsigned __int128 num  = t * sum;                   /* n^2 * sum            */
    unsigned __int128 res128 = num / (unsigned __int128)453600;
    return (seq_t)(unsigned long long)res128;
}
#endif
