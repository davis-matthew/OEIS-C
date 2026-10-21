/*
 * A224135  Number of 4 X n 0..1 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 6 n^3 + 26 n^2 + 54 n - 15)/3 for n > 2 a(1) = 16,
 *            a(2) = 81
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)81;

    /* 64‑bit intermediate is enough for n up to ~1e4 (the tested range). */
    long long nn  = (long long)n;
    long long n2  = nn * nn;
    long long n3  = n2 * nn;
    long long n4  = n3 * nn;

    long long num = n4 + 6 * n3 + 26 * n2 + 54 * nn - 15;
    return (seq_t)(num / 3);
}
