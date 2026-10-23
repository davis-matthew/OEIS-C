/*
 * A181282  a(n) is the number of associate Rota-Baxter words in one
 *          idempotent generator x and one idempotent operator P of degree n.
 *          Such words are Rota-Baxter words that begin and/or end with x, and
 *          P is applied n times in the word.
 * https://oeis.org/A181282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-1)*Catalan(n). Recurrence: a(0) = 1, a(1) = 3,
 *            (n+1)*a(n) = 4*(2*n-1)*a(n-1) (n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* base values */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)3;

    seq_t result = (seq_t)3;              /* a(1) */
    for (idx_t i = 2u; i <= n; ++i)
    {
        seq_t i_seq  = (seq_t)i;
        seq_t factor = (seq_t)4 * ((seq_t)2 * i_seq - (seq_t)1); /* 4*(2i-1) */
        seq_t denom  = i_seq + (seq_t)1;                         /* i+1 */

#ifdef __cplusplus
        /* If seq_t is an integral type use a 128‑bit intermediate if available */
        if constexpr (std::is_integral<seq_t>::value) {
#if defined(__SIZEOF_INT128__)
            __uint128_t tmp = (__uint128_t)result * (__uint128_t)factor;
            result = (seq_t)(tmp / denom);
#else
            result = (result * factor) / denom;
#endif
        } else { /* arbitrary‑precision seq_t */
            result = (result * factor) / denom;
        }
#else
        /* In C all supported seq_t values are integral types */
#if defined(__SIZEOF_INT128__)
        __uint128_t tmp = (__uint128_t)result * (__uint128_t)factor;
        result = (seq_t)(tmp / denom);
#else
        result = (result * factor) / denom;
#endif
#endif
    }
    return result;
}
