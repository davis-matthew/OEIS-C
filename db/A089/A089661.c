/*
 * A089661  a(n) = S1(n,4), where S1(n,t) = Sum_{k=0..n} k^t * Sum_{j=0..k}
 *          binomial(n,j).
 * https://oeis.org/A089661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-5)*n*(93*n^3 + 132*n^2 + 53*n - 38)/15 (OEIS
 *            A089661, a(n) = S1(n,4))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 39, u128 n <= 96,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

#if defined(__cplusplus)
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    static const seq_t init_vals[6] = {0, 2, 67, 764, 5492, 30304};

    if (n <= 5) return init_vals[n];

#if defined(__cplusplus)
    /* If seq_t is an arithmetic type (e.g. uint8_t..uint64_t) use
       unsigned __int128 for intermediate calculations.  Otherwise (an
       arbitrary‑precision type) rely on big_pow. */
    if constexpr (std::is_arithmetic<seq_t>::value) {
        unsigned __int128 ns  = (unsigned __int128)n;
        unsigned __int128 ns1 = ns + 1;
        unsigned __int128 poly  = 93 * ns * ns * ns
                                + 132 * ns * ns
                                + 53 * ns
                                - 38;
        unsigned __int128 num   = ns * ns1 * poly;
        unsigned __int128 pow2  = (unsigned __int128)1 << (n - 5);
        unsigned __int128 val   = (num * pow2) / 15;
        return (seq_t)val;
    } else {
        /* seq_t is a big‑integer type; use big_pow for the power of two. */
        seq_t ns  = (seq_t)n;
        seq_t ns1 = ns + 1;
        seq_t poly  = 93 * ns * ns * ns
                    + 132 * ns * ns
                    + 53 * ns
                    - 38;
        seq_t num   = ns * ns1 * poly;
        seq_t pow2  = big_pow((seq_t)2, (seq_t)(n - 5));
        seq_t val   = (num * pow2) / 15;
        return val;
    }
#else
    /* C (no type traits) – assume arithmetic seq_t */
    unsigned __int128 ns  = (unsigned __int128)n;
    unsigned __int128 ns1 = ns + 1;
    unsigned __int128 poly  = 93 * ns * ns * ns
                            + 132 * ns * ns
                            + 53 * ns
                            - 38;
    unsigned __int128 num   = ns * ns1 * poly;
    unsigned __int128 pow2  = (unsigned __int128)1 << (n - 5);
    unsigned __int128 val   = (num * pow2) / 15;
    return (seq_t)val;
#endif
}
