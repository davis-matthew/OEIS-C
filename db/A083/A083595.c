/*
 * A083595  a(n) = (7*2^n - 4(-1)^n)/3.
 * https://oeis.org/A083595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (7*2^n - 4(-1)^n)/3
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 64‑bit seq_t: use 128‑bit arithmetic when the result still fits */
    if (sizeof(seq_t) <= 8 && n < 63) {
        unsigned __int128 pow2 = (unsigned __int128)1 << n;       /* 2^n  */
        unsigned __int128 numer = pow2 * (unsigned __int128)7;   /* 7*2^n */
        if ((n & 1) == 0)  /* n even: subtract 4 */
            numer -= 4;
        else               /* n odd: add 4    */
            numer += 4;
        return (seq_t)(numer / 3);
    }

    /* arbitrary‑precision seq_t: use the big‑integer power routine */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);   /* 2^n  */
    seq_t term  = pow2 * (seq_t)7;              /* 7*2^n */
    if ((n & 1) == 0)
        term -= (seq_t)4;                      /* n even   */
    else
        term += (seq_t)4;                      /* n odd    */
    return term / (seq_t)3;
}
