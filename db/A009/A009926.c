/*
 * A009926  Coordination sequence for CaF2(2), Ca position.
 * https://oeis.org/A009926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-2*(-5+(-1)^n)-5*(-3+(-1)^n)*n^2)/4 for n>0, a(0) = 1
 * Range:     u8 n <= 8, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    int parity = (n & 1) ? -1 : 1;               /* (-1)^n  */
    long long n64 = (long long)n;                 /* n^2 fits 32‑bit for n ≤ 10 000 */
    long long part1 = -2LL * ((-5LL) + parity);   /* -2*(-5+(-1)^n) */
    long long part2 = -5LL * ((-3LL) + parity) * (n64 * n64); /* -5*(-3+(-1)^n)*n^2 */
    long long num   = part1 + part2;              /* numerator of division by 4 */

    return (seq_t)(num / 4);
}
