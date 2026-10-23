/*
 * A197764  Ceiling((4n+1/n)^n).
 * https://oeis.org/A197764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = ceil((4*n + 1/n)^n) with offset 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Computes A197764: ceil((4*n + 1/n)^n).  For 64‑bit seq_t  we use 128‑bit
   intermediates for n ≤ 11 (the largest n whose value still fits in 64 bits).
   For larger n the 64‑bit implementation may overflow – such terms are not
   required by the 64‑bit build.  In the arbitrary‑precision build the
   computation uses big_pow, which handles very large integers. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; value below 1 is unused */

    /* 64‑bit seq_t: handle up to n = 11 with 128‑bit intermediates. */
    if (sizeof(seq_t) <= 8) {
        if (n <= 11) {
            unsigned __int128 nb = n;
            unsigned __int128 base = nb * nb * 4 + 1;   /* 4*n^2 + 1   */
            unsigned __int128 p = 1;
            unsigned __int128 q = 1;
            for (idx_t i = 0; i < n; ++i) {
                p *= base;
                q *= nb;
            }
            unsigned __int128 num = p + q - 1;
            return (seq_t)(num / q);
        }
        /* For n > 11 the 64‑bit result would overflow; those terms are not
           used in the 64‑bit build.  We fall back to the generic routine. */
        seq_t base = ((seq_t)n * (seq_t)n) * 4 + 1;
        seq_t p = big_pow(base, (seq_t)n);
        seq_t q = big_pow((seq_t)n, (seq_t)n);
        seq_t num = p + q - 1;
        return num / q;
    }

    /* Arbitrary‑precision seq_t */
    seq_t base = ((seq_t)n * (seq_t)n) * 4 + 1;
    seq_t p = big_pow(base, (seq_t)n);
    seq_t q = big_pow((seq_t)n, (seq_t)n);
    seq_t num = p + q - 1;
    return num / q;
}
