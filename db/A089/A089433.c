/*
 * A089433  Number of noncrossing connected graphs on n nodes having exactly
 *          two interior faces.
 * https://oeis.org/A089433
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * binomial(3n-3, n-4) / 2 (OEIS A089433, offset 4)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Euclidean algorithm for arbitrary-precision seq_t */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is undefined – return 0. */
    if (n < 4)
        return 0;

    seq_t result = 2;                     /* a(4) = 2 */
    if (n == 4)
        return result;

    for (idx_t m = 5; m <= n; ++m)
    {
        /* Compute numerator and denominator of the recurrence factor */
        seq_t num = 3 * (seq_t)(3 * m - 4) * (seq_t)(3 * m - 5);
        seq_t den = 2 * (seq_t)(2 * m + 1) * (seq_t)(m - 4);

        /* Reduce to keep intermediate values small */
        seq_t g1 = seq_gcd(result, den);
        result /= g1;
        den /= g1;

        seq_t g2 = seq_gcd(num, den);
        num /= g2;
        den /= g2;

        /* Multiply result by num and divide by den.
           Use 64‑bit or arbitrary‑precision arithmetic as appropriate. */
#ifdef __cplusplus
        if constexpr (std::is_integral<seq_t>::value)
        {
            /* seq_t is an integral type – use 128‑bit intermediate. */
            unsigned __int128 tmp = (unsigned __int128)result * (unsigned __int128)num;
            result = (seq_t)(tmp / den);
        }
        else
        {
            /* seq_t is a big‑integer class – rely on built‑in operators. */
            seq_t tmp = result * num;
            result = tmp / den;
        }
#else
        /* C mode – seq_t is an unsigned integer type. */
        unsigned __int128 tmp = (unsigned __int128)result * (unsigned __int128)num;
        result = (seq_t)(tmp / den);
#endif
    }

    return result;
}
