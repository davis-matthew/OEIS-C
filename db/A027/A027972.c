/*
 * A027972  T(n, 2n-10), T given by A027960.
 * https://oeis.org/A027972
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{10} c[i] * binom(n + 5 - i, 10) where the
 *            coefficient array c is { 1, -7, 22, -37, 32, 1, -32, 37, -22, 7,
 *            -1 }. This follows from the generating function x^5*(1 -7*x
 *            +22*x^2 -37*x^3 +32*x^4 +x^5 -32*x^6 +37*x^7 -22*x^8 +7*x^9
 *            -x^10)/(1-x)^11. The implementation uses an integer‑on...
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 44, u64 n <= 384,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#ifdef __cplusplus
#  include <type_traits>
#endif

static seq_t binom10(idx_t n)
{
    /* binomial coefficient C(n,10); returns 0 for n<10 */
    if (n < 10) return (seq_t)0;
#ifdef __cplusplus
    if constexpr (std::is_integral<seq_t>::value && sizeof(seq_t) <= 8) {
        unsigned __int128 res = 1;
        for (int i = 1; i <= 10; ++i) {
            res = res * (unsigned long long)(n - 10 + i);
            res /= (unsigned long long)i;
        }
        return (seq_t)res;
    } else {
        /* seq_t is an arbitrary‑precision type or >64‑bit;
           perform the calculation using seq_t operations only. */
        seq_t res = (seq_t)1;
        for (int i = 1; i <= 10; ++i) {
            res = res * (seq_t)(n - 10 + i);
            res = res / (seq_t)i;
        }
        return res;
    }
#else
    /* Plain C implementation: use unsigned __int128 for safety. */
    unsigned __int128 res = 1;
    for (int i = 1; i <= 10; ++i) {
        res = res * (unsigned long long)(n - 10 + i);
        res /= (unsigned long long)i;
    }
    return (seq_t)res;
#endif
}

seq_t a(idx_t n)
{
    /* Offset 5: the sequence is defined for n ≥ 5. */
    if (n < 5) return (seq_t)0;

    static const int coeffs[11] = {
        1, -7, 22, -37, 32, 1, -32, 37, -22, 7, -1
    };

#ifdef __cplusplus
    if constexpr (std::is_integral<seq_t>::value && sizeof(seq_t) <= 8) {
        unsigned __int128 sum = 0;
        for (int i = 0; i < 11; ++i) {
            idx_t m = n + 5 - (idx_t)i;          /* m = n + 5 - i   */
            seq_t bin = binom10(m);
            unsigned __int128 term = (unsigned __int128)bin *
                                     (unsigned long long)(coeffs[i] >= 0 ? coeffs[i] : -coeffs[i]);
            if (coeffs[i] >= 0)
                sum += term;
            else
                sum -= term;
        }
        return (seq_t)sum;
    } else {
        /* seq_t is not a small integral type: use seq_t arithmetic. */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < 11; ++i) {
            idx_t m = n + 5 - (idx_t)i;
            seq_t bin = binom10(m);
            seq_t coeff = (seq_t)(coeffs[i] >= 0 ? coeffs[i] : -coeffs[i]);
            seq_t term = bin * coeff;
            if (coeffs[i] >= 0)
                sum += term;
            else
                sum -= term;
        }
        return sum;
    }
#else
    unsigned __int128 sum = 0;
    for (int i = 0; i < 11; ++i) {
        idx_t m = n + 5 - (idx_t)i;
        seq_t bin = binom10(m);
        unsigned __int128 term = (unsigned __int128)bin *
                                 (unsigned long long)(coeffs[i] >= 0 ? coeffs[i] : -coeffs[i]);
        if (coeffs[i] >= 0)
            sum += term;
        else
            sum -= term;
    }
    return (seq_t)sum;
#endif
}
