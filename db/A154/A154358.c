/*
 * A154358  a(n) = 1250*n^2 - 1800*n + 649.
 * https://oeis.org/A154358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1250*n^2 - 1800*n + 649
 * Range:     u8 none, u16 n <= 7, u32 n <= 1854, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 64‑bit intermediate to avoid overflow on 32‑bit seq_t builds. */
    unsigned long long N = n;
    unsigned long long val = 1250ULL * N * N - 1800ULL * N + 649ULL;
    return (seq_t)val;
}
