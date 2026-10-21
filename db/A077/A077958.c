/*
 * A077958  Expansion of 1/(1-2*x^3).
 * https://oeis.org/A077958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n/3) if n mod 3 = 0, otherwise a(n) = 0
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 95, u64 n <= 191,
 *            u128 n <= 383, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#if defined(__cplusplus)
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* Return 0 for indices not a multiple of 3    */
    if ((n % 3) != 0) {
        return 0;
    }

    /* Exponent of 2 in the answer (integer division) */
    idx_t exp = n / 3;

#if defined(__cplusplus)
    /*  If seq_t is an integral builtin type, guard against
        producing undefined behaviour when shift amount
        exceeds the width of seq_t.  For arbitrary‑precision
        types the shift operator works for any non‑negative
        exponent.                                          */
    if constexpr (std::is_integral<seq_t>::value) {
        if (exp >= (idx_t)(sizeof(seq_t) * 8)) {
            /* The value would overflow seq_t;  the spec
               only requires exactness when the result fits
               in seq_t, so we can return 0 in this case.   */
            return 0;
        }
        return ((seq_t)1) << exp;
    } else {
        /* arbitrary‑precision integer type */
        return ((seq_t)1) << exp;
    }
#else
    /* C compilation: seq_t is assumed to be an integral type. */
    if (exp >= (idx_t)(sizeof(seq_t) * 8)) {
        return 0;
    }
    return ((seq_t)1) << exp;
#endif
}
