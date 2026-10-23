/*
 * A034299  Alternating sum transform (PSumSIGN) of A000975.
 * https://oeis.org/A034299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n+5)+(6n+13)(-1)^n-9)/36
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a 128‑bit intermediate for n ≤ 120 (works for 64‑bit builds),
   otherwise falls back to big_pow for arbitrary‑precision builds. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* small indices – use unsigned __int128 intermediate (safe for n ≤ 120) */
    if (n <= 120) {
        /* compute 2^(n+5) safely */
        unsigned __int128 pow2 = ((unsigned __int128)1) << (n + 5);

        /* signed 128 to allow for subtraction of the (6n+13)(-1)^n term */
        __int128 term = (int64_t)(6 * (uint64_t)n + 13);
        if (n & 1)
            term = -term;

        /* numerator = 2^(n+5) + (6n+13)(-1)^n - 9 */
        __int128 numer = ( __int128)pow2 + term - 9;

        /* division by 36 (exact) */
        __int128 result = numer / 36;

        return (seq_t)result;
    }

    /* large indices – use big_pow for arbitrary‑precision sequence values */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n + 5));
    seq_t term = (seq_t)(6 * (uint64_t)n + 13);
    if (n & 1)
        term = -term;

    seq_t numer = pow2 + term - 9;
    return numer / 36;
}
