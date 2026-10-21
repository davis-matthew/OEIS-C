/*
 * A060073  a(n) = (n^(n-1)-1)/(n-1)^2.
 * https://oeis.org/A060073
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^(n-1)-1)/(n-1)^2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 18, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute a(n) for n >= 2 */
    if (n < 2u) return 0;   /* undefined for n < 2; return 0 */

    idx_t m = n - 1u;

    if (sizeof(seq_t) <= 8) {
        /* 64‑bit or smaller builds – use 128‑bit arithmetic for safety */
        unsigned __int128 base = (unsigned __int128)n;
        unsigned __int128 pow = 1;
        for (idx_t i = 1; i <= m; ++i) pow *= base;
        unsigned __int128 numer = pow - 1;
        unsigned __int128 denom = (unsigned __int128)m * m;
        unsigned __int128 res = numer / denom;
        return (seq_t)res;
    } else {
        /* Arbitrary‑precision builds – rely on the provided big_pow */
        seq_t bn = (seq_t)n;
        seq_t power = big_pow(bn, bn - 1);
        seq_t numer = power - 1;
        seq_t denom = (bn - 1) * (bn - 1);
        return numer / denom;
    }
}
