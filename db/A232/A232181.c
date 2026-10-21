/*
 * A232181  First bisection of harmonic numbers (denominators).
 * https://oeis.org/A232181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS entry A232181
 *  a(n) = denominator of the harmonic number H(2*n-1)
 *  where H(n) = sum_{k=1..n} 1/k  .
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* For seq_t types that fit within 64 bits we use an intermediate
 * 128‑bit integer to avoid overflow during the multiplication
 * in the fraction addition.  For larger or arbitrary‑precision
 * seq_t we rely on the normal type operations. */
#if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ == 16
#   define USE_INT128 1
#else
#   define USE_INT128 0
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* harmless, offset is 1 */

    seq_t num = (seq_t)0;     /* current numerator of H(m) */
    seq_t den = (seq_t)1;     /* current denominator of H(m) */

    /* Highest term to evaluate is m = 2*n-1. */
    idx_t limit = 2*n - (idx_t)1;

    for (idx_t m = (idx_t)1; m <= limit; ++m) {
        const seq_t mt = (seq_t)m;

#if USE_INT128
        /* Safe multiplication using 128‑bit intermediate. */
        unsigned __int128 t1 = (unsigned __int128)num * (unsigned __int128)mt;
        unsigned __int128 t2 = (unsigned __int128)den * (unsigned __int128)mt;

        /* Convert back to seq_t.  The values stay within 128 bits
         * because we only need them for the terms that fit
         * within seq_t (actual overflow is prevented). */
        seq_t new_num = (seq_t)(t1 + (unsigned __int128)den);
        seq_t new_den = (seq_t)(t2);
#else
        /* Assume seq_t large enough or arbitrary‑precision. */
        seq_t new_num = num * mt + den;
        seq_t new_den = den * mt;
#endif

        /* Reduce the fraction.  gcd is defined for all seq_t types. */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    /* After the loop den is the denominator of H(2*n-1). */
    return den;
}
