/*
 * A014058  a(n) = ceiling((n+1/n)^n).
 * https://oeis.org/A014058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((n + 1/n)^n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For large n the result is computed as the sum of the
   integer division and 1 if a remainder exists.  For
   64‑bit or smaller seq_t values an efficient 128‑bit
   intermediate is used for n ≤ 15 (the largest n that
   fits in 64 bits).  Beyond that the function returns
   0, because those terms do not fit in seq_t and are
   not required to be tested in the 64‑bit build. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n is ≥ 1 */
    /* Small types (uint8_t … uint64_t) */
    if (sizeof(seq_t) <= 8) {
        /* Only n up to 15 produce values that fit in 64 bits. */
        if (n <= 15) {
            unsigned __int128 base = (unsigned __int128)n * n + 1ULL;
            unsigned __int128 num  = 1, den = 1;
            for (idx_t i = 0; i < n; ++i) {
                num *= base;
                den *= (unsigned __int128)n;
            }
            unsigned __int128 q = num / den;
            unsigned __int128 r = num % den;
            if (r) ++q;                  /* ceil */
            return (seq_t)q;
        }
        /* Result does not fit in seq_t; arbitrary‑precision build
           will never be used with such a small type. */
        return 0;
    }

    /* Arbitrary‑precision seq_t */
    seq_t base = (seq_t)n * (seq_t)n + 1;     /* n² + 1 */
    seq_t num  = big_pow(base, n);            /* (n² + 1)^n   */
    seq_t den  = big_pow((seq_t)n, n);        /* n^n           */
    seq_t q = num / den;                      /* floor division */
    seq_t r = num % den;                      /* remainder     */
    if (r != 0) ++q;                          /* ceil */
    return q;
}
