/*
 * A081215  a(n) = (n^(n+1)+(-1)^n)/(n+1)^2.
 * https://oeis.org/A081215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^(n+1)+(-1)^n)/(n+1)^2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* The calculation depends on whether seq_t is a built‑in
       integer type (size <= 8 bytes) or an arbitrary‑precision
       big‑integer class.  In the former case we use 128‑bit
       intermediates to avoid overflow; in the latter we
       rely on the native multiplication operator. */
    if (sizeof(seq_t) <= 8) {
        /* ----- 128‑bit intermediates for native integer types ----- */
        __uint128_t pow = 1;
        for (idx_t i = 0; i < n + 1; ++i)
            pow *= (unsigned __int128)n;

        /* Add or subtract 1 depending on the parity of n. */
        if ((n & 1U) == 0)
            pow += 1;          /* n even  -> +1 */
        else
            pow -= 1;          /* n odd   -> -1 */

        /* Denominator (n+1)^2. */
        __uint128_t denom = (__uint128_t)(n + 1) * (n + 1);

        /* Integer division – the result is exact. */
        return (seq_t)(pow / denom);
    } else {
        /* -----  Arbitrary‑precision path ----- */
        seq_t pow = 1;
        for (idx_t i = 0; i < n + 1; ++i)
            pow *= (seq_t)n;

        /* Add or subtract 1 depending on the parity of n. */
        if ((n & 1U) == 0)
            pow += 1;          /* n even  -> +1 */
        else
            pow -= 1;          /* n odd   -> -1 */

        /* Denominator (n+1)^2. */
        seq_t denom = (seq_t)(n + 1);
        denom *= (seq_t)(n + 1);

        /* Integer division – exact. */
        return pow / denom;
    }
}
