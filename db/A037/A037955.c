/*
 * A037955  a(n) = binomial(n, floor(n/2)-1).
 * https://oeis.org/A037955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, floor(n/2)-1)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 67, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0, a(1) = 0 */

#include <stdint.h>
#if defined(__cplusplus)
#include <type_traits>
#endif

/* helper for the C++ build that chooses the correct multiplication route */
#ifdef __cplusplus
static inline seq_t multiply_divide(seq_t res, idx_t num, idx_t den)
{
    if constexpr (std::is_integral<seq_t>::value) {
        /* 64‑bit (or smaller) types – use 128‑bit intermediate to avoid overflow */
        return (seq_t)(((__uint128_t)res * num) / den);
    } else {
        /* arbitrary‑precision types – the built‑in operators handle large values */
        return res * num / den;
    }
}
#endif

seq_t a(idx_t n)
{
    /* first terms are zero */
    if (n < 2) return 0;

    /* k = floor(n/2)-1 */
    idx_t k = n / 2;
    k = k - 1;

    /* use the smaller half for efficiency */
    idx_t kk = k;
    if (kk > n - kk) kk = n - kk;

    seq_t result = 1;
    for (idx_t i = 1; i <= kk; ++i) {
        idx_t numerator   = n - kk + i;   /* current factor from the top row   */
        idx_t denominator = i;            /* current divisor                   */
#ifdef __cplusplus
        result = multiply_divide(result, numerator, denominator);
#else
        /* C build: seq_t is a built‑in unsigned type – use 128‑bit intermediate */
        result = (seq_t)(((__uint128_t)result * numerator) / denominator);
#endif
    }
    return result;
}
