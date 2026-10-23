/*
 * A143413  Apéry-like numbers for the constant e: a(n) = 1/(n-1)!*Sum_{k =
 *          0..n+1} (-1)^k*C(n+1,k)*(2*n-k)! for n >= 1.
 * https://oeis.org/A143413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = -1, a(1) = 1; for n ≥ 2, (n-1)²·a(n) – n²·a(n‑2) =
 *            (2n‑1)(2n²–2n+1)·a(n‑1)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif


seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)1;

    seq_t a_nm2 = (seq_t)-1;   /* a(0) */
    seq_t a_nm1 = (seq_t)1;    /* a(1) */
    seq_t a_n;

#ifdef __cplusplus
    /* Use __int128 for built‑in integral types; otherwise fall back to
       the natural seq_t operators which work with arbitrary‑precision
       integers. */
    if constexpr (std::is_integral_v<seq_t>) {
        for (idx_t i = 2; i <= n; ++i) {
            __int128 ii   = i;                  /* 128‑bit intermediate */
            __int128 n2   = ii * ii;             /* i² */
            __int128 term1 = (__int128)a_nm2 * n2;
            __int128 coef1 = 2 * ii - 1;
            __int128 coef2 = 2 * n2 - (2 * ii) + 1;
            __int128 term2 = (__int128)a_nm1 * coef1 * coef2;
            __int128 denom = (ii - 1) * (ii - 1);
            __int128 sum   = term1 + term2;
            a_n = (seq_t)(sum / denom);
            a_nm2 = a_nm1;
            a_nm1 = a_n;
        }
    } else {
        /* seq_t is an arbitrary‑precision type; use the overloaded
           operators so that all intermediate values stay exact. */
        for (idx_t i = 2; i <= n; ++i) {
            seq_t i_seq = (seq_t)i;
            seq_t n2     = i_seq * i_seq;
            seq_t term1  = a_nm2 * n2;
            seq_t coef1  = (seq_t)(2 * i_seq) - 1;
            seq_t coef2  = (seq_t)(2 * n2) - (seq_t)(2 * i_seq) + 1;
            seq_t term2  = a_nm1 * coef1 * coef2;
            seq_t denom  = (i_seq - 1) * (i_seq - 1);
            seq_t sum    = term1 + term2;
            a_n = sum / denom;      /* exact division */
            a_nm2 = a_nm1;
            a_nm1 = a_n;
        }
    }
#else
    /* Pure C build – seq_t is a native integer type.  Use __int128
       intermediate to avoid overflow. */
    for (idx_t i = 2; i <= n; ++i) {
        __int128 ii   = i;
        __int128 n2   = ii * ii;           /* i² */
        __int128 term1 = (__int128)a_nm2 * n2;
        __int128 coef1 = 2 * ii - 1;
        __int128 coef2 = 2 * n2 - (2 * ii) + 1;
        __int128 term2 = (__int128)a_nm1 * coef1 * coef2;
        __int128 denom = (ii - 1) * (ii - 1);
        __int128 sum   = term1 + term2;
        a_n = (seq_t)(sum / denom);
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
#endif

    return a_n;
}
