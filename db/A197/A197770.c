/*
 * A197770  Ceiling((10*n+1/n)^n).
 * https://oeis.org/A197770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((10*n + 1/n)^n) = ((10*n^2+1)^n + n^n - 1) / n^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used by OEIS sequence */

    /* If the underlying integer type fits in 8 bytes, use 128‑bit
       arithmetic to avoid overflow for small n (the only n whose
       result fits in 64 bits).  For larger underlying types the
       arbitrary‑precision big_pow function is used. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sn = n;                     /* n as 128‑bit integer */
        unsigned __int128 base = 10 * sn * sn + 1;     /* 10*n^2 + 1 */
        unsigned __int128 B = 1;                      /* (10*n^2+1)^n */
        for (idx_t i = 0; i < n; ++i) B *= base;

        unsigned __int128 D = 1;                      /* n^n */
        for (idx_t i = 0; i < n; ++i) D *= sn;

        unsigned __int128 numer = B + D - 1;          /* (B + D - 1) */
        seq_t res = (seq_t)(numer / D);              /* ceil(B/D) */
        return res;
    } else {
        /* Generic arbitrary‑precision case.  big_pow works with the
           arbitrary‑precision type defined in the build. */
        seq_t sn = (seq_t)n;
        seq_t base = (seq_t)10 * sn * sn + (seq_t)1;
        seq_t B = big_pow(base, sn);          /* (10*n^2+1)^n */
        seq_t D = big_pow(sn, sn);           /* n^n */
        seq_t numer = B + D - (seq_t)1;
        seq_t res = numer / D;               /* ceil(B/D) */
        return res;
    }
}
