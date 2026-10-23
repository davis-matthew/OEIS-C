/*
 * A135758  Catalan numbers at triangular positions: a(n) = A000108(n(n+1)/2).
 * https://oeis.org/A135758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Catalan( n(n+1)/2 )
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, just in case */

static seq_t _catalan(idx_t k)
{
    seq_t c = 1;
    idx_t i;
    for (i = 1; i <= k; ++i) {
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
        /* Use 128‑bit intermediate to avoid overflow on 64‑bit builds */
        unsigned __int128 tmp = (unsigned __int128)c * (4*i-2);
        c = (seq_t)(tmp / (i+1));
#else
        /* Use the seq_t operations directly (works for arbitrary‑precision C++) */
        c = c * (4*i-2) / (i+1);
#endif
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n * ((seq_t)n + 1) / 2;
    return _catalan((idx_t)k);
}
