/*
 * A094972  a(n) = floor(11^n/2^n).
 * https://oeis.org/A094972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(11^n/2^n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For small seq_t sizes we compute using 128‑bit arithmetic to avoid
       overflow, relying on the fact that the result still fits in seq_t.
       For arbitrary precision seq_t we use the provided big_pow routine. */
    if (sizeof(seq_t) <= 8) {                 /* uint8_t, uint16_t, uint32_t or uint64_t */
        __int128 num = 1;
        for (idx_t i = 0; i < n; ++i)
            num *= 11;
        return (seq_t)((__int128)num >> n);
    } else {                                  /* seq_t is 128‑bit or a big‑int class */
        seq_t num = big_pow((seq_t)11, (seq_t)n);
        seq_t den = pow2(n);
        return num / den;
    }
}
