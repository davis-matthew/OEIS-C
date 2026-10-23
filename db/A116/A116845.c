/*
 * A116845  Number of permutations of length n which avoid the patterns 231,
 *          12534.
 * https://oeis.org/A116845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Fibonacci(2*n)/5 + Lucas(2*n-3)*n/5
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

#ifdef __cplusplus
#include <type_traits>

/* Big‑integer (or non‑integral) implementation */
static inline seq_t compute_bigint(idx_t n)
{
    seq_t f2n = fib(2 * n);
    seq_t f2n4 = fib(2 * n - 4);
    seq_t f2n2 = fib(2 * n - 2);
    seq_t part2 = f2n4 + f2n2;
    part2 = part2 * (seq_t)n;
    seq_t numerator = f2n + part2;
    seq_t result = numerator / (seq_t)5 + (seq_t)1;
    return result;
}

/* Unsigned‑integer implementation using 128‑bit intermediates */
template<typename T>
static inline T compute_int(idx_t n)
{
    T f2n   = fib(2 * n);
    T f2n4  = fib(2 * n - 4);
    T f2n2  = fib(2 * n - 2);
    unsigned __int128 part2 = (unsigned __int128)f2n4 + (unsigned __int128)f2n2;
    part2 *= (unsigned __int128)n;
    unsigned __int128 numerator = (unsigned __int128)f2n + part2;
    unsigned __int128 res = numerator / 5 + 1;
    return (T)res;
}

seq_t a(idx_t n)
{
    static const seq_t init[5] = {1, 2, 5, 14, 41};
    if (n <= 5) return init[n - 1];
    if constexpr (std::is_unsigned<seq_t>::value) {
        return compute_int<seq_t>(n);
    } else {
        return compute_bigint(n);
    }
}

#else /* C implementation */

static inline seq_t compute_bigint(idx_t n)
{
    seq_t f2n   = fib(2 * n);
    seq_t f2n4  = fib(2 * n - 4);
    seq_t f2n2  = fib(2 * n - 2);
    seq_t part2 = f2n4 + f2n2;
    part2 = part2 * (seq_t)n;
    seq_t numerator = f2n + part2;
    seq_t result = numerator / (seq_t)5 + (seq_t)1;
    return result;
}

static inline seq_t compute_int(idx_t n)
{
    seq_t f2n   = fib(2 * n);
    seq_t f2n4  = fib(2 * n - 4);
    seq_t f2n2  = fib(2 * n - 2);
    unsigned __int128 part2 = (unsigned __int128)f2n4 + (unsigned __int128)f2n2;
    part2 *= (unsigned __int128)n;
    unsigned __int128 numerator = (unsigned __int128)f2n + part2;
    unsigned __int128 res = numerator / 5 + 1;
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    static const seq_t init[5] = {1, 2, 5, 14, 41};
    if (n <= 5) return init[n - 1];
    return _Generic((seq_t)0,
        unsigned char:    compute_int(n),
        unsigned short:   compute_int(n),
        unsigned int:     compute_int(n),
        unsigned long:    compute_int(n),
        unsigned long long: compute_int(n),
        default:          compute_bigint(n)
    );
}
#endif
