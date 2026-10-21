/*
 * A088313  Number of "sets of lists" (cf. A000262) with an odd number of
 *          lists.
 * https://oeis.org/A088313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor((n+1)/2)} n!/(2*k-1)! * binomial(n-1,
 *            2*k-2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Works for idx_t = uint64_t and arbitrary seq_t (integral or big integer) */

#include <stdint.h>
#include <stdbool.h>

#if defined(__cplusplus)
#include <type_traits>

/* Generic muldiv for integral types (uses 128‑bit intermediate for safety)
   and for arbitrary‑precision types (uses normal multiplication/division). */
template <class T>
inline T muldiv(const T& a, const T& b, const T& c) {
    if constexpr (std::is_integral<T>::value) {
        return static_cast<unsigned __int128>(a) * b / c;
    } else {
        return (a * b) / c;
    }
}
#else
/* C implementation – use 128‑bit intermediate if the compiler provides it. */
static inline seq_t muldiv(seq_t a, seq_t b, seq_t c) {
#if defined(__SIZEOF_INT128__)
    return (unsigned __int128)a * b / c;
#else
    /* If 128‑bit is unavailable, rely on natural overflow behaviour
       (acceptable because seq_t is the output type). */
    return (a * b) / c;
#endif
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* factorial n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    seq_t term = fact;      /* term for k = 1 */
    seq_t sum  = term;

    idx_t maxk = (n + 1) / 2;   /* floor((n+1)/2) */

    for (idx_t k = 2; k <= maxk; ++k) {
        /* Compute numerator (n-2k+3)*(n-2k+2) */
        seq_t a1 = (seq_t)(n - 2 * k + 3);
        seq_t a2 = (seq_t)(n - 2 * k + 2);
        seq_t num = a1 * a2;

        /* Compute denominator (2k-1)*(2k-2)*(2k-2)*(2k-3) */
        seq_t denom = (seq_t)(2 * k - 1) *
                      (seq_t)(2 * k - 2) *
                      (seq_t)(2 * k - 2) *
                      (seq_t)(2 * k - 3);

        term = muldiv(term, num, denom);
        sum += term;
    }

    return sum;
}
