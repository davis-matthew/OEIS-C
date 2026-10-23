/*
 * A182626  a(n) = Hypergeometric([-n, n], [1], 2).
 * https://oeis.org/A182626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} (-2)^k * binomial(n,k) * binomial(n+k-1,k)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 13, i64 n <= 26, i128 n <= 50,
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

seq_t a(idx_t n)
{
    seq_t sum = 1;                /* a(0) = 1 */
    if (n == 0U) return sum;

#if defined(__cplusplus)
    constexpr bool big_int = !std::is_integral<seq_t>::value;
#else
    #define big_int 0
#endif

    if (big_int) {
        /* seq_t is an arbitrary‑precision type – normal integer arithmetic works. */
        seq_t t = 1;              /* t_0 */
        for (idx_t k = 1; k <= n; ++k) {
            seq_t numer = t * ((seq_t)(-2));
            numer = numer * ((seq_t)(n - k + 1));
            numer = numer * ((seq_t)(n + k - 1));
            seq_t denom = ((seq_t)(k)) * ((seq_t)(k));
            t = numer / denom;
            sum += t;
        }
        return sum;
    } else {
#if defined(__SIZEOF_INT128__)
        /* seq_t is a built‑in integer type – use __int128 for safe intermediate values. */
        __int128 sum128 = 1;
        __int128 t128   = 1;
        for (idx_t k = 1; k <= n; ++k) {
            __int128 num = t128 * -2;
            num *= (n - k + 1);
            num *= (n + k - 1);
            __int128 denom = (__int128)k * (__int128)k;
            t128 = num / denom;
            sum128 += t128;
        }
        return (seq_t)sum128;
#else
        /* Fallback if __int128 is not available – may overflow for large n. */
        seq_t t = 1;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t numer = t * ((seq_t)(-2));
            numer = numer * ((seq_t)(n - k + 1));
            numer = numer * ((seq_t)(n + k - 1));
            seq_t denom = ((seq_t)(k)) * ((seq_t)(k));
            t = numer / denom;
            sum += t;
        }
        return sum;
#endif
    }
}
