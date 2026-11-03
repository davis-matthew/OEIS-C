/*
 * A129006  a(n) = (n^3 + n^2)*6^n.
 * https://oeis.org/A129006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n^2) * 6^n (valid for n >= 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 43,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 999 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for safety return 0 when n == 0 */
    if (n == 0) return 0;

    /* Compute 6^n */
    seq_t power = 1;
    for (idx_t i = 0; i < n; ++i)
        power = mul_small(power, 6);

    /* Compute (n^3 + n^2) = n^2 * (n + 1) */
    seq_t n_ts   = (seq_t)n;          /* cast first to seq_t */
    seq_t n_sq   = n_ts * n_ts;       /* n^2           */
    seq_t factor = n_sq * (n_ts + 1); /* n^3 + n^2     */

    return factor * power;
}
