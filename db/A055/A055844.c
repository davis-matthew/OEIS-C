/*
 * A055844  a(n) = (5*n + 9)*binomial(n+8, 8)/9.
 * https://oeis.org/A055844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5*n + 9)*binomial(n+8, 8)/9
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 36, u64 n <= 475, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t bin       = binomial(n + 8, 8);          /* C(n+8,8)                */
    seq_t multiplier= (seq_t)5 * n + 9;             /* 5*n + 9                  */
    /* Use 128‑bit intermediate to avoid overflow on 64‑bit builds   */
    unsigned __int128 prod = (unsigned __int128)multiplier *
                             (unsigned __int128)bin;   /* (5*n+9)*C(n+8,8)        */
    return (seq_t)(prod / 9);              /* divide by 9, truncate to seq_t */
}
