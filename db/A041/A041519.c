/*
 * A041519  Denominators of continued fraction convergents to sqrt(276).
 * https://oeis.org/A041519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041519: Denominators of continued fraction convergents to sqrt(276)
   Recurrence:  a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=13, a(6)=31, a(7)=75,
                a(8)=106, a(9)=181, a(10)=287, a(11)=468, a(12)=15263, a(13)=15731,
                a(14)=30994, a(15)=46725, a(16)=77719, a(17)=202163, a(18)=482045,
                a(19)=1166253, a(20)=1648298, a(21)=2814551, a(22)=4462849, a(23)=7277400
   a(n) = 15550*a(n‑12) – a(n‑24) for n ≥ 24 */
#include <stddef.h>
#include <stdlib.h>

#if defined(__cplusplus)
/* C++ implementation – use uint128_t when seq_t is an unsigned integer type */
#include <type_traits>
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if constexpr (std::is_unsigned_v<seq_t>)
        return (seq_t)((__uint128_t)x * m);
    else
        return x * (seq_t)m;
}
#else
/* C implementation – use unsigned __int128 for the intermediate product */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    return (seq_t)((unsigned __int128)x * (unsigned __int128)m);
}
#endif

static const seq_t init[24] = {
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)13, (seq_t)31, (seq_t)75,
    (seq_t)106, (seq_t)181, (seq_t)287, (seq_t)468, (seq_t)15263, (seq_t)15731,
    (seq_t)30994, (seq_t)46725, (seq_t)77719, (seq_t)202163, (seq_t)482045,
    (seq_t)1166253, (seq_t)1648298, (seq_t)2814551, (seq_t)4462849, (seq_t)7277400
};

seq_t a(idx_t n)
{
    if (n < 24)
        return init[n];

    seq_t buf[24];
    for (size_t i = 0; i < 24; ++i)
        buf[i] = init[i];

    for (idx_t i = 24; i <= n; ++i)
    {
        size_t idx12 = (i - 12) % 24;
        size_t idx24 = (i - 24) % 24;
        seq_t prod = mul_small(buf[idx12], 15550u);
        buf[i % 24] = prod - buf[idx24];
    }

    return buf[n % 24];
}
