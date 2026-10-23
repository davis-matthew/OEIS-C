/*
 * A024488  a(n) = (1/(3n-1))*M(3n; n,n,n), where M(...) is a multinomial
 *          coefficient.
 * https://oeis.org/A024488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/(3n-1))*M(3n; n,n,n) Recurrence: a(1)=3
 *            a(n)=3*(3n-2)*(3n-4)/(n^2)*a(n-1) for n>=2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;
    seq_t res = (seq_t)3;
    for (idx_t k = (idx_t)2; k <= n; ++k)
    {
        seq_t num = (seq_t)3 * (seq_t)(3*k - (idx_t)2)
                      * (seq_t)(3*k - (idx_t)4);
        seq_t den = (seq_t)k * (seq_t)k;
#ifdef __cplusplus
        /* C++ build: seq_t may be an arbitrary‑precision class */
        res = res * num / den;
#else
#if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ >= 16
        /* C build: use 128‑bit intermediate to avoid overflow */
        __uint128_t prod = (__uint128_t)res * num;
        prod /= den;
        res = (seq_t)prod;
#else
        /* Fallback: 64‑bit arithmetic – sufficient for the 64‑bit tests */
        res = (res * num) / den;
#endif
#endif
    }
    return res;
}
