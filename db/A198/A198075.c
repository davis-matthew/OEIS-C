/*
 * A198075  Round((n+1/n)^10).
 * https://oeis.org/A198075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Round((n + 1/n)^10)
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 84, u128 n <= 439,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; for n < 1 the sequence is undefined, but the
       caller will not request such an index. */
    /* For small n (up to about 80 for 64‑bit seq_t) we can compute
       the value exactly using 128‑bit arithmetic.  For larger or
       arbitrary‑precision seq_t we fall back to big_pow. */
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit intermediate arithmetic. */
        unsigned __int128 N = (unsigned __int128)(seq_t)n;
        unsigned __int128 base = N * N + 1;
        unsigned __int128 num = 1;
        unsigned __int128 den = 1;
        for (int i = 0; i < 10; ++i) {
            num *= base;
            den *= N;
        }
        unsigned __int128 half = den >> 1;          /* den/2 */
        unsigned __int128 sum  = num + half;        /* (num + den/2) */
        unsigned __int128 res128 = sum / den;       /* floor((num+den/2)/den) */
        return (seq_t)res128;
    } else {
        /* seq_t is an arbitrary‑precision type.  Use big_pow for safety. */
        seq_t N   = (seq_t)n;
        seq_t base = N * N + 1;
        seq_t num = big_pow(base, (seq_t)10);
        seq_t den = big_pow(N,    (seq_t)10);
        seq_t half = den >> 1;                      /* den/2 */
        seq_t sum  = num + half;                    /* (num + den/2) */
        return sum / den;                           /* rounded value */
    }
}
