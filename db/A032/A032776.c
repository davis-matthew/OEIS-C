/*
 * A032776  Integer quotients n(n+1)(n+2)...(n+6) / (n+(n+1)+(n+2)+...+(n+6)).
 * https://oeis.org/A032776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Integer quotients n(n+1)(n+2)(n+4)(n+5)(n+6)/7
   where the underlying integer n is the n-th integer that is
   NOT congruent to 4 modulo 7 (i.e. the quotient is defined
   only for those n).
*/
#include <stdint.h>

seq_t a(idx_t m)
{
    /* Map the sequence index m to the underlying integer n₀
       (skip the values where n ≡ 4 (mod 7)). */
    idx_t block  = m / 6;          /* number of full 6‑term blocks */
    idx_t offset = m % 6;          /* position inside the block   */
    idx_t base   = 7 * block;      /* start of the block          */
    /* Skip the integer whose offset is 4 (i.e. base+4) */
    idx_t n0 = base + offset + (offset >= 4 ? 1 : 0);

    /* The six contributing factors (the middle one has been removed) */
    seq_t f[6];
    f[0] = (seq_t)n0;
    f[1] = (seq_t)(n0 + 1);
    f[2] = (seq_t)(n0 + 2);
    f[3] = (seq_t)(n0 + 4);
    f[4] = (seq_t)(n0 + 5);
    f[5] = (seq_t)(n0 + 6);

    /* Reduce one factor that is divisible by 7 */
    for (int i = 0; i < 6; ++i) {
        if (f[i] % 7 == 0) {
            f[i] /= 7;
            break;
        }
    }

    /* Compute the product using 128‑bit arithmetic to avoid overflow
       in the 64‑bit builds while still fitting in seq_t for known use
       cases. */
    unsigned __int128 prod = 1;
    for (int i = 0; i < 6; ++i) {
        prod *= (unsigned __int128)f[i];
    }

    return (seq_t)prod;
}
