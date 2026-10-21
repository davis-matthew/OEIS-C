/*
 * A055660  Number of (2,2; n,n)-partitions of a chain of length n^2 + n.
 * https://oeis.org/A055660
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(2*n-3)*(n-3)*(2*n-5)!/((n-3)!^2*n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#ifndef __SIZEOF_INT128__
# define __SIZEOF_INT128__ 0
#endif

/* Helper: multiply and divide (a*b)/c using a big enough temporary type when possible */
#if defined(__cplusplus)
#include <type_traits>
static inline seq_t muldiv(seq_t a, seq_t b, seq_t c)
{
    if constexpr (std::is_integral_v<seq_t>) {
#   if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ > 0
        unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)b;
        return (seq_t)(prod / c);
#   else
        return a * b / c;
#   endif
    } else {
        return a * b / c;
    }
}
#else /* C11 */
static inline seq_t muldiv_builtin(seq_t a, seq_t b, seq_t c)
{
#if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ > 0
    unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)b;
    return (seq_t)(prod / c);
#else
    return a * b / c;
#endif
}
static inline seq_t muldiv_custom(seq_t a, seq_t b, seq_t c)
{
    return a * b / c;
}
static inline seq_t muldiv(seq_t a, seq_t b, seq_t c)
{
    return _Generic((a),
        unsigned char: muldiv_builtin,
        unsigned short: muldiv_builtin,
        unsigned int: muldiv_builtin,
        unsigned long: muldiv_builtin,
        unsigned long long: muldiv_builtin,
        default: muldiv_custom)(a,b,c);
}
#endif

/* binomial coefficient C(N,K) using multiplicative method */
static seq_t binomial(idx_t N, idx_t K)
{
    if (K > N - K)
        K = N - K;
    seq_t result = 1;
    for (idx_t i = 1; i <= K; ++i) {
        result = muldiv(result,
                        (seq_t)(N - K + i),
                        (seq_t)i);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;   /* undefined below offset */

    idx_t N = 2 * n - 5;
    idx_t K = n - 3;

    seq_t bin = binomial(N, K);

    /* Remaining multiplicative factor: 2*(2n-3)*(n-3)*(n-2) */
    seq_t temp = 2 *
                 (seq_t)(2 * n - 3) *
                 (seq_t)(n - 3) *
                 (seq_t)(n - 2);

    return muldiv(bin, temp, (seq_t)n);
}
