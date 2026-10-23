/*
 * A034494  a(n) = (7^n+1)/2.
 * https://oeis.org/A034494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A034494: a(n) = (7^n + 1) / 2
 * Recurrence: a(0) = 1; a(n) = 7 * a(n-1) - 3   (for n ≥ 1)
 */

#ifndef __cplusplus
/* C implementation (unsigned integer seq_t) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t val = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        unsigned __int128 tmp = (unsigned __int128)val * 7u;
        tmp -= 3u;
        val = (seq_t)tmp;
    }
    return val;
}
#else
/* C++ implementation (arbitrary‑precision seq_t) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t val = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        val = val * (seq_t)7 - (seq_t)3;
    }
    return val;
}
#endif
