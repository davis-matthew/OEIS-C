/*
 * A071296  a(n) is the least m such that a period of the continued fraction
 *          expansion of sqrt(m) is 1,1,1,...,1,1,1,Z and there are n ones in
 *          the period (Z is 2*floor(sqrt(m))). If no such m exists, a(n) = 0.
 * https://oeis.org/A071296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n ≡ 2 (mod 3); otherwise a(n) = x² + (F(n‑1) +
 *            2·x·F(n)) / F(n+1), where F(k) is the k‑th Fibonacci number
 *            (F(1)=1, F(2)=1) and x = (F(n+1)+1)/2. The implementation uses
 *            the helper fib_pair(n, &Fn, &Fn1) which returns F(n) and F(n+1).
 *            For integral seq_t types 128‑bit intermediates are...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib_pair.h"

#ifdef __cplusplus
#include <type_traits>
/* 128‑bit multiplication for integral types, identity for others */
template <typename T>
static inline T mul128_impl(T a, T b) noexcept
{
    if constexpr (std::is_integral<T>::value) {
        return (T)((unsigned __int128)a * (unsigned __int128)b);
    } else {
        return a * b;
    }
}
#define MUL(a,b) mul128_impl(a,b)

/* 128‑bit numerator for term2, otherwise native arithmetic */
template <typename T>
static inline T compute_term2_impl(T Fn_minus1, T Fn, T Fn1, T x) noexcept
{
    if constexpr (std::is_integral<T>::value) {
        unsigned __int128 num = (unsigned __int128)Fn_minus1
                              + (unsigned __int128)2 * (unsigned __int128)x *
                                (unsigned __int128)Fn;
        return (T)(num / (unsigned __int128)Fn1);
    } else {
        return (Fn_minus1 + (T)2 * x * Fn) / Fn1;
    }
}
#define COMPUTE_TERM2(Fnm1,Fn,Fn1,x) compute_term2_impl(Fnm1,Fn,Fn1,x)
#else
/* For C: always integral seq_t; use 128‑bit intermediates */
#define MUL(a,b) ((seq_t)((unsigned __int128)(a) * (unsigned __int128)(b)))
#define COMPUTE_TERM2(Fnm1,Fn,Fn1,x) ((seq_t)(( (unsigned __int128)(Fnm1) \
    + (unsigned __int128)2 * (unsigned __int128)(x) * (unsigned __int128)(Fn) ) \
    / (unsigned __int128)(Fn1) ))
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* outside the offset */

    /* no solution for n ≡ 2 (mod 3) */
    if ((n % 3ULL) == 2ULL) return 0;

    seq_t Fn, Fn1;                           /* Fn  = F(n),  Fn1 = F(n+1) */
    fib_pair(n, &Fn, &Fn1);

    seq_t Fn_minus1 = Fn1 - Fn;             /* F(n-1) */
    seq_t x = (Fn1 + 1) / 2;                /* (F(n+1)+1)/2 */

    seq_t term1 = MUL(x, x);                 /* x² */

    seq_t term2 = COMPUTE_TERM2(Fn_minus1, Fn, Fn1, x);

    return term1 + term2;
}
