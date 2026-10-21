/*
 * A051376  Number of Boolean functions of n variables and rank 4 from Post
 *          class F(5,inf).
 * https://oeis.org/A051376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (8^n - 7^n - 6*4^n + 6*3^n + 11*2^n - 17)/6
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow, pow_simple_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses 128‑bit arithmetic when seq_t is smaller
   than or equal to 8 bytes; otherwise it uses the generic big‑pow
   routine from the utils library. */
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/pow_simple_uint.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 by the caller. */
    if (sizeof(seq_t) <= 8) {
        /* Use 128‑bit intermediate arithmetic for small seq_t types. */
        __int128 pow8 = pow_simple_uint(8, n);
        __int128 pow7 = pow_simple_uint(7, n);
        __int128 pow4 = pow_simple_uint(4, n);
        __int128 pow3 = pow_simple_uint(3, n);
        __int128 pow2 = pow_simple_uint(2, n);

        __int128 pos = pow8 + (__int128)6 * pow3 + (__int128)11 * pow2;
        __int128 neg = pow7 + (__int128)6 * pow4 + 17;
        __int128 num = pos - neg;
        return (seq_t)(num / 6);
    } else {
        /* Use the arbitrary‑precision big_pow routine for larger seq_t types. */
        seq_t pow8 = big_pow((seq_t)8, (seq_t)n);
        seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
        seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
        seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
        seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

        seq_t pos = pow8 + ((seq_t)6 * pow3) + ((seq_t)11 * pow2);
        seq_t neg = pow7 + ((seq_t)6 * pow4) + (seq_t)17;
        seq_t num = pos - neg;
        return num / (seq_t)6;
    }
}
