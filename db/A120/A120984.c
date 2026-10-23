/*
 * A120984  Number of ternary trees with n edges and having no vertices of
 *          degree 1. A ternary tree is a rooted tree in which each vertex has
 *          at most three children and each child of a vertex is designated as
 *          its left or middle or right child.
 * https://oeis.org/A120984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = [18*(3*n+1)*(n-1)*a(n-2)+135*(n-1)*(n-2)*a(n-3)
   - 3*(3*n+2)*(n-1)*a(n-1)] / [2*(2*n+3)*(n+1)]
   Initial values: a(0)=1, a(1)=0, a(2)=3 */

#include <stdint.h>

#ifdef __cplusplus
#include <type_traits>

static inline seq_t mul_small_bigint2(seq_t a, unsigned int b, unsigned int c)
{
    return a * (seq_t)b * (seq_t)c;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)3;

    seq_t a_nm3 = (seq_t)1; /* a(0) */
    seq_t a_nm2 = (seq_t)0; /* a(1) */
    seq_t a_nm1 = (seq_t)3; /* a(2) */
    seq_t res = a_nm1;

    constexpr bool small = std::is_integral_v<seq_t> && sizeof(seq_t) <= 8;

    for (idx_t i = 3; i <= n; ++i) {
        if constexpr (small) {
            __uint128_t term1 = (__uint128_t)a_nm2 * 18 * (3*i+1) * (i-1);
            __uint128_t term2 = (__uint128_t)a_nm3 * 135 * (i-1) * (i-2);
            __uint128_t term3 = (__uint128_t)a_nm1 * 3 * (3*i+2) * (i-1);
            __uint128_t num   = term1 + term2 - term3;
            __uint128_t den   = 2 * (2*i+3) * (i+1);
            res = (seq_t)(num / den);
        } else {
            seq_t term1 = a_nm2 * (seq_t)18 * (seq_t)(3*i+1) * (seq_t)(i-1);
            seq_t term2 = a_nm3 * (seq_t)135 * (seq_t)(i-1) * (seq_t)(i-2);
            seq_t term3 = a_nm1 * (seq_t)3 * (seq_t)(3*i+2) * (seq_t)(i-1);
            seq_t num   = term1 + term2 - term3;
            seq_t den   = (seq_t)2 * (seq_t)(2*i+3) * (seq_t)(i+1);
            res = num / den;
        }
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = res;
    }
    return res;
}

#else /* C implementation */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)3;

    seq_t a_nm3 = (seq_t)1; /* a(0) */
    seq_t a_nm2 = (seq_t)0; /* a(1) */
    seq_t a_nm1 = (seq_t)3; /* a(2) */
    seq_t res = a_nm1;

    for (idx_t i = 3; i <= n; ++i) {
        __uint128_t term1 = (__uint128_t)a_nm2 * 18 * (3*i+1) * (i-1);
        __uint128_t term2 = (__uint128_t)a_nm3 * 135 * (i-1) * (i-2);
        __uint128_t term3 = (__uint128_t)a_nm1 * 3 * (3*i+2) * (i-1);
        __uint128_t num   = term1 + term2 - term3;
        __uint128_t den   = 2 * (2*i+3) * (i+1);
        res = (seq_t)(num / den);

        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = res;
    }
    return res;
}

#endif
