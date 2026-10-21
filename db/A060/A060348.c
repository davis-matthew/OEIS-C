/*
 * A060348  a(n) = n^n * (n^2 - 1)/24.
 * https://oeis.org/A060348
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n * (n^2 - 1)/24
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;   /* below offset – undefined */

    /* ----- 64‑bit or smaller seq_t ----- */
    if (sizeof(seq_t) <= 8) {
        /* Use 128‑bit intermediate arithmetic to avoid overflow
           for all n whose result actually fits in 64 bits. */
        unsigned __int128 base = n;
        unsigned __int128 pow   = 1;
        for (idx_t i = 0; i < n; ++i)
            pow *= base;                     /* compute n^n */

        unsigned __int128 numer = pow * (base * base - 1);
        unsigned __int128 res   = numer / 24;
        return (seq_t)res;
    }

    /* ----- Larger or arbitrary‑precision seq_t ----- */
    seq_t nn = (seq_t)n;
    seq_t pw = big_pow(nn, nn);                    /* n^n */
    seq_t numerator = pw * (nn * nn - (seq_t)1);   /* n^n * (n^2-1) */
    seq_t res = numerator / (seq_t)24;
    return res;
}
